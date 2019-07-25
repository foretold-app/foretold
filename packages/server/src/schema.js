const graphql = require("graphql");
const { resolver } = require("graphql-sequelize");
const { applyMiddleware } = require('graphql-middleware');

const models = require("./models");
const resolvers = require('./resolvers');

const types = require('./types');
const { stats } = require('./types/stats');

const { permissions } = require('./authorizers');
const { middlewares } = require('./middlewares');

const schema = new graphql.GraphQLSchema({
  types: [
    types.channels.channel,
    types.channelMemberships.channelsMembership,
    types.bots.bot,
    types.users.user,
  ],

  query: new graphql.GraphQLObjectType({
    name: 'Query',
    fields: {

      permissions: {
        type: types.permissions.permissions,
        args: {
          channelId: { type: graphql.GraphQLString },
          measurableId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.permissions.all,
      },

      user: {
        type: types.users.user,
        args: {
          id: { type: graphql.GraphQLString },
          auth0Id: { type: graphql.GraphQLString },
        },
        resolve: resolvers.users.one,
      },

      users: {
        type: graphql.GraphQLNonNull(graphql.GraphQLList(types.users.user)),
        resolve: resolver(models.User),
      },

      measurement: {
        type: types.measurements.measurement,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.measurements.one,
      },

      measurements: {
        type: types.measurements.measurementsConnection,
        args: {
          ...types.common.connectionArguments,
          measurableId: { type: graphql.GraphQLString },
          agentId: { type: graphql.GraphQLString },
          competitorType: { type: graphql.GraphQLList(types.measurementCompetitorType.measurementCompetitorType) },
          findInDateRange: { type: types.measurements.measurementsInDateRangeInput },
          notTaggedByAgent: { type: graphql.GraphQLString },
        },
        resolve: resolvers.measurements.all,
      },

      measurable: {
        type: types.measurables.measurable,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.measurables.one,
      },

      measurables: {
        type: types.measurables.measurablesConnection,
        args: {
          ...types.common.connectionArguments,
          creatorId: { type: graphql.GraphQLString },
          seriesId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLString },
          measuredByAgentId: { type: graphql.GraphQLString },
          states: { type: graphql.GraphQLList(types.measurables.measurableState) },
          isArchived: { type: graphql.GraphQLList(types.common.isArchived) },
          resultOrLatestMeasurementForAgentId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.measurables.all,
      },

      bot: {
        type: types.bots.bot,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.bots.one,
      },

      bots: {
        type: types.bots.botsConnection,
        args: {
          ...types.common.connectionArguments,
          ownerId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.bots.all,
      },

      agent: {
        type: types.agents.agent,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolver(models.Agent),
      },

      agents: {
        args: {
          excludeChannelId: { type: graphql.GraphQLString },
          types: { type: graphql.GraphQLList(types.agents.agentType) },
        },
        type: graphql.GraphQLNonNull(graphql.GraphQLList(types.agents.agent)),
        resolve: resolvers.agents.all,
      },

      series: {
        type: types.series.series,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.series.one,
      },

      seriesCollection: {
        type: graphql.GraphQLNonNull(graphql.GraphQLList(types.series.series)),
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
          channelMemberId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.channels.all,
      },

      stats: {
        type: new graphql.GraphQLNonNull(stats),
        resolve: () => true,
      },

      authentication: {
        type: graphql.GraphQLNonNull(types.authentications.authenticationJwt),
        args: {
          auth0jwt: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          auth0accessToken: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          authToken: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.authentications.exchangeToken,
      },
    }
  }),

  mutation: new graphql.GraphQLObjectType({
    name: 'Mutation',
    fields: {

      measurementCreate: {
        type: types.measurements.measurement,
        args: {
          input: { type: graphql.GraphQLNonNull(types.measurements.measurementCreateInput) },
        },
        resolve: resolvers.measurements.create,
      },

      measurableCreate: {
        type: types.measurables.measurable,
        args: {
          input: { type: graphql.GraphQLNonNull(types.measurables.measurableCreateInput) },
        },
        resolve: resolvers.measurables.create,
      },

      seriesCreate: {
        type: types.series.series,
        args: {
          input: { type: graphql.GraphQLNonNull(types.series.seriesCreateInput) },
        },
        resolve: resolvers.series.create,
      },

      measurableArchive: {
        type: types.measurables.measurable,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.measurables.archive,
      },

      measurableUnarchive: {
        type: types.measurables.measurable,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.measurables.unarchive,
      },

      measurableUpdate: {
        type: types.measurables.measurable,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: { type: graphql.GraphQLNonNull(types.measurables.measurableUpdateInput) },
        },
        resolve: resolvers.measurables.update,
      },

      userUpdate: {
        type: types.users.user,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: { type: graphql.GraphQLNonNull(types.users.userUpdateInput) },
        },
        resolve: resolvers.users.update,
      },

      channelUpdate: {
        type: types.channels.channel,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: { type: graphql.GraphQLNonNull(types.channels.channelInput) },
        },
        resolve: resolvers.channels.update,
      },

      channelCreate: {
        type: types.channels.channel,
        args: {
          input: { type: graphql.GraphQLNonNull(types.channels.channelInput) },
        },
        resolve: resolvers.channels.create,
      },

      channelMembershipCreate: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: { type: graphql.GraphQLNonNull(types.channelMemberships.channelMembershipRoleInput) },
        },
        resolve: resolvers.channelMemberships.create,
      },

      channelMembershipRoleUpdate: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: { type: graphql.GraphQLNonNull(types.channelMemberships.channelMembershipRoleInput) },
        },
        resolve: resolvers.channelMemberships.update,
      },

      channelMembershipDelete: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: { type: graphql.GraphQLNonNull(types.channelMemberships.channelMembershipDeleteInput) },
        },
        resolve: resolvers.channelMemberships.remove,
      },

      leaveChannel: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: { type: graphql.GraphQLNonNull(types.channelMemberships.joiningChannelInput) },
        },
        resolve: resolvers.channelMemberships.leave,
      },

      joinChannel: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: { type: graphql.GraphQLNonNull(types.channelMemberships.joiningChannelInput) },
        },
        resolve: resolvers.channelMemberships.join,
      },

      botCreate: {
        type: types.bots.bot,
        args: {
          input: { type: graphql.GraphQLNonNull(types.bots.botInput) },
        },
        resolve: resolvers.bots.create,
      },

      botUpdate: {
        type: types.bots.bot,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: { type: graphql.GraphQLNonNull(types.bots.botInput) },
        },
        resolve: resolvers.bots.update,
      },

      botTokenRefresh: {
        type: types.authentications.authenticationToken,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.bots.tokenRefresh,
      },

      preferenceUpdate: {
        type: types.preferences.preference,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: { type: graphql.GraphQLNonNull(types.preferences.preferenceUpdateInput) },
        },
        resolve: resolvers.preferences.update,
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
