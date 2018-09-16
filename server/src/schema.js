import * as models from "./models/index";
import * as _ from "lodash";
import {
  makeAggregation
} from "./services/measurable/MakeAggregation"
import {
  resolver,
  attributeFields
} from "graphql-sequelize";
import {
  GraphQLObjectType,
  GraphQLNonNull,
  GraphQLFloat,
  GraphQLList,
  GraphQLSchema,
  GraphQLInt,
  GraphQLString,
  GraphQLInputObjectType
} from "graphql";
import * as GraphQLJSON from "graphql-type-json";

function capitalizeFirstLetter(str) {
  return str.charAt(0).toUpperCase() + str.slice(1);
}

const maybePluralize = (count, noun, suffix = 's') =>
  `${noun}${count !== 1 ? suffix : ''}`;

const generateReferences = (model) => {
  let all = {};
  //Was trying to figure this bit out.
  // const references = model.associations.
  const associations = _.toArray(model.associations);
  associations.map(r => {
    const hasMany = r.associationType === "HasMany"
    const toMany = r.associationType === "BelongsToMany"
    const type = (hasMany || toMany) ? getType[r.as]() : getType[maybePluralize(2, r.as)]()
    all[r.as] = {
      type: hasMany ? GraphQLNonNull(new GraphQLList(type)) : type,
      resolve: resolver(model[capitalizeFirstLetter(r.as)])
    };
  });
  return all;
};

const makeObjectType = (model, references, extraFields = {}) =>
  new GraphQLObjectType({
    name: model.name,
    description: model.name,
    fields: () =>
      _.assign(
        attributeFields(model),
        generateReferences(model, references),
        extraFields
      )
  });

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
}

const simpleResolver = (type, model) => ({
  type: new GraphQLNonNull(GraphQLList(type)),
  resolve: resolver(model)
})

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
  }
  return fields;
}

const schema = new GraphQLSchema({
  query: new GraphQLObjectType({
    name: 'Query',
    fields: {
      ...modelResolvers("user", "users", getType.Users(), models.User),
      ...modelResolvers("measurement", "measurements", getType.Measurements(), models.Measurement),
      ...modelResolvers("measurable", "measurables", getType.Measurables(), models.Measurable),
      ...modelResolvers("bot", "bots", getType.Bots(), models.Bot),
      ...modelResolvers("agent", "agent", getType.Agents(), models.Agent),
      createMeasurement: {
        type: getType.Measurements(),
        args: _.pick(attributeFields(models.Measurement), ['percentile25', 'percentile50', 'percentile75', 'measurableId', 'isAggregation']),
        resolve: async (__, {
          percentile25,
          percentile50,
          percentile75,
          measurableId,
          agentId,
          isAggregation
        }) => {
          const newMeasurement = await models.Measurement.create({
            percentile25,
            percentile50,
            percentile75,
            measurableId,
          agentId,
            isAggregation
          })
          const measurable = await newMeasurement.getMeasurable();
          return newMeasurement
        }
      },
    },
  }),
});

export {
  schema
};
