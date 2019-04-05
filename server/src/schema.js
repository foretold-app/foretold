const _ = require('lodash');

const graphql = require("graphql");
const { attributeFields, resolver } = require("graphql-sequelize");
const { applyMiddleware } = require('graphql-middleware');

const models = require("./models");
const resolvers = require('./resolvers');

const types = require('./types');
const { stats } = require('./types/stats');
const { filterr } = require('./types/filterr');

const { permissions } = require('./authorizers');
const { middlewares } = require('./middlewares');

const schema = new graphql.GraphQLSchema({
  query: new graphql.GraphQLObjectType({
    name: 'Query',
    fields: {

      user: {
        type: types.userType,
        args: {
          id: { type: graphql.GraphQLString },
          auth0Id: { type: graphql.GraphQLString },
        },
        resolve: resolvers.users.one,
      },

      users: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.userType)),
        resolve: resolver(models.User),
      },

      measurement: {
        type: types.measurementType,
        args: _.pick(attributeFields(models.Measurement), ['id']),
        resolve: resolvers.measurements.one,
      },

      measurements: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.measurementType)),
        args: {
          measurableId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.measurements.all,
      },

      measurable: {
        type: types.measurables.measurable,
        args: _.pick(attributeFields(models.Measurable), ['id']),
        resolve: resolvers.measurables.one,
      },

      measurables: {
        type: new graphql.GraphQLNonNull(new graphql.GraphQLList(types.measurables.measurable)),
        args: {
          offset: { type: graphql.GraphQLInt },
          limit: { type: graphql.GraphQLInt },
          creatorId: { type: graphql.GraphQLString },
          seriesId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLString },
          states: { type: graphql.GraphQLList(types.measurables.measurableState) },
        },
        resolve: resolvers.measurables.all,
      },

      bot: {
        type: types.botType,
        args: _.pick(attributeFields(models.Bot), ['id']),
        resolve: resolver(models.Bot),
      },

      bots: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.botType)),
        resolve: resolver(models.Bot),
      },

      agent: {
        type: types.agents.agent,
        args: _.pick(attributeFields(models.Agent), ['id']),
        resolve: resolver(models.Agent),
      },

      agents: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.agents.agent)),
        resolve: resolver(models.Agent),
      },

      series: {
        type: types.seriesType,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.series.one,
      },

      seriesCollection: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.seriesType)),
        args: {
          channelId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.series.all,
      },

      channel: {
        type: types.channels.channel,
        args: { id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) } },
        resolve: resolvers.channels.one,
      },

      channels: {
        type: graphql.GraphQLNonNull(graphql.GraphQLList(types.channels.channel)),
        args: {
          offset: { type: graphql.GraphQLInt },
          limit: { type: graphql.GraphQLInt },
        },
        resolve: resolvers.channels.all,
      },

      stats: {
        type: new graphql.GraphQLNonNull(stats),
        resolve: async (ops, values, options) => {
          // @todo:
          return 11;
        }
      },
    }
  }),
  mutation: new graphql.GraphQLObjectType({
    name: 'Mutation',
    fields: {

      measurementCreate: {
        type: types.measurementType,
        args: filterr(_.pick(attributeFields(models.Measurement), [
          'value', 'competitorType', 'measurableId', 'agentId', 'description'
        ])),
        resolve: resolvers.measurements.create,
      },

      measurableCreate: {
        type: types.measurables.measurable,
        args: filterr(_.pick(attributeFields(models.Measurable), [
          'name', 'labelCustom', 'valueType', 'expectedResolutionDate',
          'resolutionEndpoint', 'labelSubject', 'labelOnDate',
          'labelProperty', 'channelId',
        ])),
        resolve: resolvers.measurables.create,
      },

      seriesCreate: {
        type: types.seriesType,
        args: filterr(_.pick(attributeFields(models.Series), [
          'name', 'description', 'channelId', 'subjects', 'properties',
          'dates',
        ])),
        resolve: resolvers.series.create,
      },

      measurableArchive: {
        type: types.measurables.measurable,
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: resolvers.measurables.archive,
      },

      measurableUnarchive: {
        type: types.measurables.measurable,
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: resolvers.measurables.unarchive,
      },

      measurableUpdate: {
        type: types.measurables.measurable,
        args: filterr(_.pick(attributeFields(models.Measurable), [
          'id', 'name', 'labelCustom', 'expectedResolutionDate',
          'resolutionEndpoint', 'labelSubject', 'labelOnDate',
          'labelProperty',
        ])),
        resolve: resolvers.measurables.update,
      },

      userUpdate: {
        type: types.userType,
        args: filterr(_.pick(attributeFields(models.User), ["id", "name"])),
        resolve: resolvers.users.update,
      },

      channelUpdate: {
        type: types.channels.channel,
        args: {
          id: { type: graphql.GraphQLString },
          input: { type: new graphql.GraphQLNonNull(types.channels.channelInput) },
        },
        resolve: resolvers.channels.update,
      },

      channelCreate: {
        type: types.channels.channel,
        args: {
          input: { type: new graphql.GraphQLNonNull(types.channels.channelInput) },
        },
        resolve: resolvers.channels.create,
      },

      channelMembershipCreate: {
        type: types.channelMemberships.channelsMembership,
        args: {
          agentId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLString },
          role: { type: graphql.GraphQLNonNull(types.channelMemberships.role) },
        },
        resolve: resolvers.channelMemberships.create,
      },

      channelMembershipRoleUpdate: {
        type: types.channelMemberships.channelsMembership,
        args: {
          agentId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLString },
          role: { type: graphql.GraphQLNonNull(types.channelMemberships.role) },
        },
        resolve: resolvers.channelMemberships.update,
      },

      channelMembershipDelete: {
        type: types.channelMemberships.channelsMembership,
        args: {
          agentId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.channelMemberships.remove,
      },

    }
  })
});

const schemaWithMiddlewares = applyMiddleware(
  schema,
  middlewares,
  permissions,
);

module.exports = {
  schemaWithMiddlewares,
};
