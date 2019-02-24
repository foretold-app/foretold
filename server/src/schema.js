import * as models from "./models/index";
import * as _ from "lodash";
const Sequelize = require('sequelize')
const jwt = require('express-jwt')
const { 
  AuthenticationError, 
} = require('apollo-server');

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
  GraphQLEnumType,
  GraphQLList,
  GraphQLSchema,
  GraphQLInt,
  GraphQLString,
  GraphQLInputObjectType
} from "graphql";
import * as GraphQLJSON from "graphql-type-json";
import {notify} from "./lib/notifications";

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
    const otherTableName = r.target.tableName
    const type = getType[otherTableName]()
    all[r.as] = {
      type: hasMany ? GraphQLNonNull(new GraphQLList(type)) : type,
      resolve: resolver(model[capitalizeFirstLetter(r.as)])
    };
    // console.log(model, r.as, otherTableName, type, model[r.as], model[capitalizeFirstLetter(r.as)])
  });
  return all;
};

let competitor = GraphQLNonNull(new GraphQLEnumType({
  name: 'competitorType',
  values: {
    COMPETITIVE: {value: "COMPETITIVE"}, // The first ENUM value will be the default order. The direction will be used for `first`, will automatically be inversed for `last` lookups.
    AGGREGATION: {value:  "AGGREGATION"},
    OBJECTIVE: {value:  "OBJECTIVE"} // build and return custom order for sequelize orderBy option
  }
}))

let valueType = GraphQLNonNull(new GraphQLEnumType({
  name: 'valueType',
  values: {
    FLOAT: {value: "FLOAT"}, // The first ENUM value will be the default order. The direction will be used for `first`, will automatically be inversed for `last` lookups.
    DATE: {value:  "DATE"},
    PERCENTAGE: {value:  "PERCENTAGE"} // build and return custom order for sequelize orderBy option
  }
}))

const filterr = (fields) => {
  let newFields = {...fields}
  if (!!newFields.competitorType){
    newFields.competitorType = {type: competitor}
  }
  if (!!newFields.valueType){
    newFields.valueType = {type: valueType}
  }
  return newFields
}

const makeObjectType = (model, references, extraFields = {}) =>
  new GraphQLObjectType({
    name: model.name,
    description: model.name,
    fields: () =>
      _.assign(
        filterr(attributeFields(model)),
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

async function auth0User(auth0Id){
  let users = await models.User.findAll({
    where: {
      auth0Id: auth0Id,
    }
  })
  return users && users[0]
}

//todo: clean up user login code.

const getAuth0Id = async (options) => {
  let {ok, result} = await options.user;
  if (!ok){
    throw new Error(
      result.name
    );
  }
  let {sub} = result;
  if (!sub){
    throw new Error(
      "No User Id"
    );
  }
  let userAuth0Id = sub;
  return userAuth0Id
}

const stats = new GraphQLObjectType({
  name: "Stats",
  fields: {
    agentCount: {
      type: GraphQLInt,
      resolve: async () => {
        return await models.Agent.count();
      }
    },
    userCount: {
      type: GraphQLInt,
      resolve: async () => {
        return await models.User.count();
      }
    },
    botCount: {
      type: GraphQLInt,
      resolve: async () => {
        return await models.Bot.count();
      }
    },
    measurementCount: {
      type: GraphQLInt,
      resolve: async () => {
        return await models.Measurement.count();
      }
    },
    measurableCount: {
      type: GraphQLInt,
      resolve: async () => {
        return await models.Measurable.count();
      }
    }
  }
})

const schema = new GraphQLSchema({
  query: new GraphQLObjectType({
    name: 'Query',
    fields: {
      ...modelResolvers("user", "users", getType.Users(), models.User),
      user: {
        type: getType.Users(),
        args: {id: {type: GraphQLString}, auth0Id: {type: GraphQLString}},
        resolve: async (ops, {
          id,
          auth0Id
        }, options) => {
          let _auth0Id = await getAuth0Id(options)
          const _auth0User = await auth0User(_auth0Id);
          let user;
          if (_auth0Id && !_auth0User) {
            try {
            user = await models.User.create({auth0Id: _auth0Id, name: ""})
            } catch (e) { console.log("E", e)}
          }
          if (user){
            return user;
          } else if (id){
            user = await models.User.findById(id);
            return user
          } else if (auth0Id){
            const user = await auth0User(auth0Id)
            return user;
          }
        }
      },
      ...modelResolvers("measurement", "measurements", getType.Measurements(), models.Measurement),
      ...modelResolvers("measurable", "measurables", getType.Measurables(), models.Measurable),
      ...modelResolvers("bot", "bots", getType.Bots(), models.Bot),
      ...modelResolvers("agent", "agents", getType.Agents(), models.Agent),
      stats: {
        type: new GraphQLNonNull(stats),
        args: {},
        resolve: async (ops, {}, options) => {
          return "sdf"
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
        resolve: async (a, {
          value,
          competitorType,
          measurableId,
          description,
        }, options) => {
          let _auth0Id = await getAuth0Id(options)
          const user = await auth0User(_auth0Id);
          const newMeasurement = await models.Measurement.create({
            value,
            competitorType,
            measurableId,
            description,
            agentId: user.agentId,
          })
          let notification = await newMeasurement.creationNotification(user);
          notify(notification);
          const measurable = await newMeasurement.getMeasurable();
          return newMeasurement
        }
      },
      createMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick(attributeFields(models.Measurable), ['name', 'description', 'valueType', 'expectedResolutionDate', 'resolutionEndpoint', 'descriptionEntity', 'descriptionDate'])),
        resolve: async (__, {
          name,
          description,
          valueType,
          expectedResolutionDate,
          resolutionEndpoint,
          descriptionDate,
          descriptionEntity
        }, options) => {
          let _auth0Id = await getAuth0Id(options)
          const user = await auth0User(_auth0Id);
          const newMeasurable = await models.Measurable.create({
          name,
          valueType,
          description,
          expectedResolutionDate,
          creatorId: user.agentId,
          descriptionEntity,
          descriptionDate,
          resolutionEndpoint
          })
          let notification = await newMeasurable.creationNotification(user);
          notify(notification)
          return newMeasurable
        }
      },
      archiveMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: async (__, {
          id,
        }, options) => {
          let _auth0Id = await getAuth0Id(options)
          const user = await auth0User(_auth0Id);
          let measurable = await models.Measurable.findById(id);
          if (measurable.creatorId !== user.agentId){
            throw new Error("User does not have permission")
          }
          return measurable.archive()
        }
      },
      unArchiveMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: async (__, {
          id,
        }, options) => {
          let _auth0Id = await getAuth0Id(options)
          const user = await auth0User(_auth0Id);
          let measurable = await models.Measurable.findById(id);
          if (measurable.creatorId !== user.agentId){
            throw new Error("User does not have permission")
          }
          return measurable.unarchive()
        }
      },
      editMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick(attributeFields(models.Measurable), ['id','name', 'description', 'expectedResolutionDate', 'resolutionEndpoint', 'descriptionEntity', 'descriptionDate'])),
        resolve: async (__, {
          id,
          name,
          description,
          expectedResolutionDate,
          descriptionEntity,
          descriptionDate,
          resolutionEndpoint
        }, options) => {
          let _auth0Id = await getAuth0Id(options)
          const user = await auth0User(_auth0Id);
          let measurable = await models.Measurable.findById(id);
          if (measurable.creatorId !== user.agentId){
            throw new Error("User does not have permission")
          }
          let notification = await measurable.updateNotifications(user, {name, description, expectedResolutionDate,resolutionEndpoint, descriptionEntity, descriptionDate});
          notify(notification);
          return measurable.update({name, description, expectedResolutionDate, resolutionEndpoint, descriptionEntity, descriptionDate})
        }
      },
      editUser: {
        type: getType.Users(),
        args: filterr(_.pick(attributeFields(models.User), ["id", "name"])),
        resolve: async (_, {
          id,
          name
        }, options) => {
          let _auth0Id = await getAuth0Id(options);
          let user = await models.User.findById(id);
          if (user && (user.auth0Id == _auth0Id)) {
            user.update({name})
          }
          return user
        }
      },
    }
    })
});

export {
  schema
};
