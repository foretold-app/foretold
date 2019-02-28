"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.schema = undefined;

var _extends = Object.assign || function (target) { for (var i = 1; i < arguments.length; i++) { var source = arguments[i]; for (var key in source) { if (Object.prototype.hasOwnProperty.call(source, key)) { target[key] = source[key]; } } } return target; };

var _index = require("./models/index");

var models = _interopRequireWildcard(_index);

var _lodash = require("lodash");

var _ = _interopRequireWildcard(_lodash);

var _MakeAggregation = require("./services/measurable/MakeAggregation");

var _graphqlSequelize = require("graphql-sequelize");

var _graphql = require("graphql");

var _graphqlTypeJson = require("graphql-type-json");

var GraphQLJSON = _interopRequireWildcard(_graphqlTypeJson);

var _notifications = require("./lib/notifications");

function _interopRequireWildcard(obj) { if (obj && obj.__esModule) { return obj; } else { var newObj = {}; if (obj != null) { for (var key in obj) { if (Object.prototype.hasOwnProperty.call(obj, key)) newObj[key] = obj[key]; } } newObj.default = obj; return newObj; } }

const Sequelize = require('sequelize');
const jwt = require('express-jwt');
const {
  AuthenticationError
} = require('apollo-server');

function capitalizeFirstLetter(str) {
  return str.charAt(0).toUpperCase() + str.slice(1);
}

const maybePluralize = (count, noun, suffix = 's') => `${noun}${count !== 1 ? suffix : ''}`;

const generateReferences = model => {
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
      type: hasMany ? (0, _graphql.GraphQLNonNull)(new _graphql.GraphQLList(type)) : type,
      resolve: (0, _graphqlSequelize.resolver)(model[capitalizeFirstLetter(r.as)])
    };
    // console.log(model, r.as, otherTableName, type, model[r.as], model[capitalizeFirstLetter(r.as)])
  });
  return all;
};

let competitor = (0, _graphql.GraphQLNonNull)(new _graphql.GraphQLEnumType({
  name: 'competitorType',
  values: {
    COMPETITIVE: { value: "COMPETITIVE" }, // The first ENUM value will be the default order. The direction will be used for `first`, will automatically be inversed for `last` lookups.
    AGGREGATION: { value: "AGGREGATION" },
    OBJECTIVE: { value: "OBJECTIVE" // build and return custom order for sequelize orderBy option
    } }
}));

let valueType = (0, _graphql.GraphQLNonNull)(new _graphql.GraphQLEnumType({
  name: 'valueType',
  values: {
    FLOAT: { value: "FLOAT" }, // The first ENUM value will be the default order. The direction will be used for `first`, will automatically be inversed for `last` lookups.
    DATE: { value: "DATE" },
    PERCENTAGE: { value: "PERCENTAGE" // build and return custom order for sequelize orderBy option
    } }
}));

const filterr = fields => {
  let newFields = _extends({}, fields);
  if (!!newFields.competitorType) {
    newFields.competitorType = { type: competitor };
  }
  if (!!newFields.valueType) {
    newFields.valueType = { type: valueType };
  }
  return newFields;
};

const makeObjectType = (model, references, extraFields = {}) => new _graphql.GraphQLObjectType({
  name: model.name,
  description: model.name,
  fields: () => _.assign(filterr((0, _graphqlSequelize.attributeFields)(model)), generateReferences(model, references), extraFields)
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
  Measurements: () => measurementType
};

const simpleResolver = (type, model) => ({
  type: new _graphql.GraphQLNonNull((0, _graphql.GraphQLList)(type)),
  resolve: (0, _graphqlSequelize.resolver)(model)
});

const modelResolvers = (name, plural, type, model) => {
  let fields = {};
  fields[name] = {
    type: type,
    args: _.pick((0, _graphqlSequelize.attributeFields)(model), ['id']),
    resolve: (0, _graphqlSequelize.resolver)(model)
  };
  fields[plural] = {
    type: new _graphql.GraphQLNonNull((0, _graphql.GraphQLList)(type)),
    resolve: (0, _graphqlSequelize.resolver)(model)
  };
  return fields;
};

async function auth0User(auth0Id) {
  let users = await models.User.findAll({
    where: {
      auth0Id: auth0Id
    }
  });
  return users && users[0];
}

//todo: clean up user login code.

const getAuth0Id = async options => {
  let { ok, result } = await options.user;
  if (!ok) {
    throw new Error(result.name);
  }
  let { sub } = result;
  if (!sub) {
    throw new Error("No User Id");
  }
  let userAuth0Id = sub;
  return userAuth0Id;
};

const stats = new _graphql.GraphQLObjectType({
  name: "Stats",
  fields: {
    agentCount: {
      type: _graphql.GraphQLInt,
      resolve: async () => {
        return await models.Agent.count();
      }
    },
    userCount: {
      type: _graphql.GraphQLInt,
      resolve: async () => {
        return await models.User.count();
      }
    },
    botCount: {
      type: _graphql.GraphQLInt,
      resolve: async () => {
        return await models.Bot.count();
      }
    },
    measurementCount: {
      type: _graphql.GraphQLInt,
      resolve: async () => {
        return await models.Measurement.count();
      }
    },
    measurableCount: {
      type: _graphql.GraphQLInt,
      resolve: async () => {
        return await models.Measurable.count();
      }
    }
  }
});

const schema = new _graphql.GraphQLSchema({
  query: new _graphql.GraphQLObjectType({
    name: 'Query',
    fields: _extends({}, modelResolvers("user", "users", getType.Users(), models.User), {
      user: {
        type: getType.Users(),
        args: { id: { type: _graphql.GraphQLString }, auth0Id: { type: _graphql.GraphQLString } },
        resolve: async (ops, {
          id,
          auth0Id
        }, options) => {
          let _auth0Id = await getAuth0Id(options);
          const _auth0User = await auth0User(_auth0Id);
          let user;
          if (_auth0Id && !_auth0User) {
            try {
              user = await models.User.create({ auth0Id: _auth0Id, name: "" });
            } catch (e) {
              console.log("E", e);
            }
          }
          if (user) {
            return user;
          } else if (id) {
            user = await models.User.findById(id);
            return user;
          } else if (auth0Id) {
            const user = await auth0User(auth0Id);
            return user;
          }
        }
      }
    }, modelResolvers("measurement", "measurements", getType.Measurements(), models.Measurement), modelResolvers("measurable", "measurables", getType.Measurables(), models.Measurable), modelResolvers("bot", "bots", getType.Bots(), models.Bot), modelResolvers("agent", "agents", getType.Agents(), models.Agent), {
      stats: {
        type: new _graphql.GraphQLNonNull(stats),
        resolve: async (ops, {}, options) => {}
      },
      measurables: {
        type: new _graphql.GraphQLNonNull(new _graphql.GraphQLList(getType.Measurables())),
        args: { offset: { type: _graphql.GraphQLInt }, limit: { type: _graphql.GraphQLInt }, channel: { type: _graphql.GraphQLString } },
        resolve: async (ops, { offset, limit, channel }, options) => {
          const mms = await models.Measurable.findAll({
            limit: limit,
            offset: offset,
            order: [['createdAt', 'DESC']],
            where: {
              channel: {
                [Sequelize.Op.eq]: channel
              },
              state: {
                [Sequelize.Op.ne]: "ARCHIVED"
              }
            }
          });
          return mms;
        }
      }
    })
  }),
  mutation: new _graphql.GraphQLObjectType({
    name: 'Mutation',
    fields: {
      createMeasurement: {
        type: getType.Measurements(),
        args: filterr(_.pick((0, _graphqlSequelize.attributeFields)(models.Measurement), ['value', 'competitorType', 'measurableId', 'agentId', 'description'])),
        resolve: async (a, {
          value,
          competitorType,
          measurableId,
          description
        }, options) => {
          let _auth0Id = await getAuth0Id(options);
          const user = await auth0User(_auth0Id);
          const newMeasurement = await models.Measurement.create({
            value,
            competitorType,
            measurableId,
            description,
            agentId: user.agentId
          });
          let notification = await newMeasurement.creationNotification(user);
          (0, _notifications.notify)(notification);
          const measurable = await newMeasurement.getMeasurable();
          return newMeasurement;
        }
      },
      createMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick((0, _graphqlSequelize.attributeFields)(models.Measurable), ['name', 'description', 'valueType', 'expectedResolutionDate', 'resolutionEndpoint', 'descriptionEntity', 'descriptionDate', 'channel'])),
        resolve: async (__, {
          name,
          description,
          valueType,
          expectedResolutionDate,
          resolutionEndpoint,
          descriptionDate,
          descriptionEntity,
          channel
        }, options) => {
          let _auth0Id = await getAuth0Id(options);
          const user = await auth0User(_auth0Id);
          const newMeasurable = await models.Measurable.create({
            name,
            valueType,
            description,
            expectedResolutionDate,
            creatorId: user.agentId,
            descriptionEntity,
            descriptionDate,
            resolutionEndpoint,
            channel
          });
          let notification = await newMeasurable.creationNotification(user);
          (0, _notifications.notify)(notification);
          return newMeasurable;
        }
      },
      archiveMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick((0, _graphqlSequelize.attributeFields)(models.Measurable), ['id'])),
        resolve: async (__, {
          id
        }, options) => {
          let _auth0Id = await getAuth0Id(options);
          const user = await auth0User(_auth0Id);
          let measurable = await models.Measurable.findById(id);
          if (measurable.creatorId !== user.agentId) {
            throw new Error("User does not have permission");
          }
          return measurable.archive();
        }
      },
      unArchiveMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick((0, _graphqlSequelize.attributeFields)(models.Measurable), ['id'])),
        resolve: async (__, {
          id
        }, options) => {
          let _auth0Id = await getAuth0Id(options);
          const user = await auth0User(_auth0Id);
          let measurable = await models.Measurable.findById(id);
          if (measurable.creatorId !== user.agentId) {
            throw new Error("User does not have permission");
          }
          return measurable.unarchive();
        }
      },
      editMeasurable: {
        type: getType.Measurables(),
        args: filterr(_.pick((0, _graphqlSequelize.attributeFields)(models.Measurable), ['id', 'name', 'description', 'expectedResolutionDate', 'resolutionEndpoint', 'descriptionEntity', 'descriptionDate'])),
        resolve: async (__, {
          id,
          name,
          description,
          expectedResolutionDate,
          descriptionEntity,
          descriptionDate,
          resolutionEndpoint
        }, options) => {
          let _auth0Id = await getAuth0Id(options);
          const user = await auth0User(_auth0Id);
          let measurable = await models.Measurable.findById(id);
          if (measurable.creatorId !== user.agentId) {
            throw new Error("User does not have permission");
          }
          let notification = await measurable.updateNotifications(user, { name, description, expectedResolutionDate, resolutionEndpoint, descriptionEntity, descriptionDate });
          (0, _notifications.notify)(notification);
          return measurable.update({ name, description, expectedResolutionDate, resolutionEndpoint, descriptionEntity, descriptionDate });
        }
      },
      editUser: {
        type: getType.Users(),
        args: filterr(_.pick((0, _graphqlSequelize.attributeFields)(models.User), ["id", "name"])),
        resolve: async (_, {
          id,
          name
        }, options) => {
          let _auth0Id = await getAuth0Id(options);
          let user = await models.User.findById(id);
          if (user && user.auth0Id == _auth0Id) {
            user.update({ name });
          }
          return user;
        }
      }
    }
  })
});

exports.schema = schema;
//# sourceMappingURL=schema.js.map