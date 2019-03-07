const {
  GraphQLInt,
  GraphQLList,
  GraphQLNonNull,
  GraphQLObjectType,
  GraphQLSchema,
  GraphQLString
} = require("graphql");
const _ = require('lodash');
const { attributeFields, resolver } = require("graphql-sequelize");

const models = require("./models");
const { measurementData, usersData, measurableData } = require('./data');
const { capitalizeFirstLetter } = require('./helpers');

const { competitor } = require('./types/competitor');
const { valueType } = require('./types/value-type');
const { stats } = require('./types/stats');

/**
 * @param model
 */
function generateReferences(model) {
  let all = {};
  //Was trying to figure this bit out.
  // const references = model.associations.
  const associations = _.toArray(model.associations);
  associations.map(r => {
    const hasMany = r.associationType === "HasMany";
    const toMany = r.associationType === "BelongsToMany";
    const otherTableName = r.target.tableName;
    const type = getType[otherTableName]();
    all[r.as] = {
      type: hasMany ? GraphQLNonNull(new GraphQLList(type)) : type,
      resolve: resolver(model[capitalizeFirstLetter(r.as)])
    };
    // console.log(model, r.as, otherTableName, type, model[r.as], model[capitalizeFirstLetter(r.as)])
  });
  return all;
}

const filterr = (fields) => {
  let newFields = { ...fields };
  if (!!newFields.competitorType) {
    newFields.competitorType = { type: competitor }
  }
  if (!!newFields.valueType) {
    newFields.valueType = { type: valueType }
  }
  return newFields
};

/**
 * @param model
 * @param references
 * @param extraFields
 * @return {GraphQLObjectType}
 */
const makeObjectType = (model, references, extraFields = {}) => {
  return new GraphQLObjectType({
    name: model.name,
    description: model.name,
    fields: () =>
      _.assign(
        filterr(attributeFields(model)),
        generateReferences(model, references),
        extraFields
      )
  });
};

const userType = makeObjectType(models.User);
const measurableType = makeObjectType(models.Measurable);
const measurementType = makeObjectType(models.Measurement);
const botType = makeObjectType(models.Bot);
const agentType = makeObjectType(models.Agent);

const getType = {
  Users: () => userType,
  Agents: () => agentType,
  Bots: () => botType,
  Measurables: () => measurableType,
  measurables: () => measurableType,
  Measurements: () => measurementType,
};

/**
 * @param name
 * @param plural
 * @param type
 * @param model
 */
const modelResolvers = (name, plural, type, model) => {
  let fields = {};
  fields[name] = {
    type: type,
    args: _.pick(attributeFields(model), ['id']),
    resolve: resolver(model)
  };
  fields[plural] = {
    type: new GraphQLNonNull(GraphQLList(type)),
    resolve: resolver(model)
  };
  return fields;
};

const schema = new GraphQLSchema({
  query: new GraphQLObjectType({
    name: 'Query',
    fields: {
      ...modelResolvers("user", "users", getType.Users(), models.User),
      user: {
        type: getType.Users(),
        args: {
          id: { type: GraphQLString },
          auth0Id: { type: GraphQLString },
        },
        resolve: async (ops, values, options) => {
          return usersData.getUser(ops, values, options);
        }
      },
      ...modelResolvers("measurement", "measurements", getType.Measurements(), models.Measurement),
      ...modelResolvers("measurable", "measurables", getType.Measurables(), models.Measurable),
      ...modelResolvers("bot", "bots", getType.Bots(), models.Bot),
      ...modelResolvers("agent", "agents", getType.Agents(), models.Agent),
      stats: {
        type: new GraphQLNonNull(stats),
        resolve: async (ops, values, options) => {
          // @todo:
          return 11;
        }
      },
      measurables: {
        type: new GraphQLNonNull(new GraphQLList(getType.Measurables())),
        args: {
          offset: { type: GraphQLInt },
          limit: { type: GraphQLInt },
          channel: { type: GraphQLString }
        },
        resolve: async (ops, values, options) => {
          return measurableData.findAll(ops, values, options);
        }
      },
    }
  }),
  mutation: new GraphQLObjectType({
    name: 'Mutation',
    fields: {
      createMeasurement: {
        type: getType.Measurements(),
        args: filterr(_.pick(attributeFields(models.Measurement), ['value', 'competitorType', 'measurableId', 'agentId', 'description'])),
        resolve: async (root, values, options) => {
          return measurementData.createMeasurement(root, values, options);
        },
      },
      createMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick(attributeFields(models.Measurable), ['name', 'description', 'valueType', 'expectedResolutionDate', 'resolutionEndpoint', 'descriptionEntity', 'descriptionDate', 'descriptionProperty', 'channel'])),
        resolve: async (root, values, options) => {
          return measurableData.createMeasurable(root, values, options);
        }
      },
      archiveMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: async (root, values, options) => {
          return measurableData.archiveMeasurable(root, values, options);
        }
      },
      unArchiveMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: async (root, values, options) => {
          return measurableData.unArchiveMeasurable(root, values, options);
        }
      },
      editMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick(attributeFields(models.Measurable), ['id', 'name', 'description', 'expectedResolutionDate', 'resolutionEndpoint', 'descriptionEntity', 'descriptionDate', 'descriptionProperty'])),
        resolve: async (root, values, options) => {
          return measurableData.editMeasurable(root, values, options);
        }
      },
      editUser: {
        type: getType.Users(),
        args: filterr(_.pick(attributeFields(models.User), ["id", "name"])),
        resolve: async (root, values, options) => {
          return usersData.editUser(root, values, options);
        }
      },
    }
  })
});

export {
  schema
};
