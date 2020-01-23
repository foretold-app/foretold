const graphql = require('graphql');
const { resolver } = require('graphql-sequelize');
const { applyMiddleware } = require('graphql-middleware');

const models = require('../models/definitions');
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
    types.agents.agent,
    types.notebooks.notebook,
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
        },
        resolve: resolvers.users.one,
      },

      // @todo: Do not use resolver. Use common interfaces of Data layer.
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

          // IDs
          measurableId: { type: graphql.GraphQLString },
          agentId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLString },
          notTaggedByAgent: { type: graphql.GraphQLString },

          competitorType: {
            type: graphql.GraphQLList(
              types.measurementCompetitorType.measurementCompetitorType,
            ),
          },
          findInDateRange: {
            type: types.measurements.measurementsInDateRangeInput,
          },
          measurableState: {
            type: graphql.GraphQLList(types.measurables.measurableState),
          },
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
          measurableIds: {
            type: graphql.GraphQLList(
              graphql.GraphQLNonNull(graphql.GraphQLString),
            ),
          },
          creatorId: { type: graphql.GraphQLString },
          seriesId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLString },
          measuredByAgentId: { type: graphql.GraphQLString },
          states: {
            type: graphql.GraphQLList(
              types.measurables.measurableState,
            ),
          },
          isArchived: { type: graphql.GraphQLList(types.common.isArchived) },
        },
        resolve: resolvers.measurables.all,
      },

      agentMeasurables: {
        type: types.agentMeasurables.agentMeasurablesConnection,
        args: {
          ...types.common.connectionArguments,
          channelId: { type: graphql.GraphQLString },
          measurableId: { type: graphql.GraphQLString },
          measurableState: {
            type: graphql.GraphQLList(
              types.measurables.measurableState,
            ),
          },
          minPredictionCountTotal: { type: graphql.GraphQLInt },
          order: { type: new graphql.GraphQLList(types.common.order) },
        },
        resolve: resolvers.agentMeasurables.all,
      },

      agentChannels: {
        type: types.agentChannels.agentChannelsConnection,
        args: {
          ...types.common.connectionArguments,
          minNumberOfPredictions: { type: graphql.GraphQLInt },
          minNumberOfQuestionsScored: { type: graphql.GraphQLInt },
          channelId: { type: graphql.GraphQLString },
          agentId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.agentChannels.all,
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

      notebook: {
        type: types.notebooks.notebook,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.notebookId) },
        },
        resolve: resolvers.notebooks.one,
      },

      notebooks: {
        type: types.notebooks.notebooksConnection,
        args: {
          ...types.common.connectionArguments,
          ownerId: { type: types.scalars.agentId },
          channelId: { type: types.scalars.channelId },
        },
        resolve: resolvers.notebooks.all,
      },

      feedItems: {
        type: types.feedItems.feedItemsConnection,
        args: {
          ...types.common.connectionArguments,
          channelId: { type: graphql.GraphQLString },
          agentId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.feedItems.all,
      },

      globalSetting: {
        type: types.globalSettings.globalSetting,
        args: {
          name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.globalSettings.one,
      },

      agent: {
        type: types.agents.agent,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        // @todo: Do not use resolver. Use common interfaces of Data layer.
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
        type: graphql.GraphQLNonNull(graphql.GraphQLList(
          types.channels.channel,
        )),
        args: {
          offset: { type: graphql.GraphQLInt },
          limit: { type: graphql.GraphQLInt },
          channelMemberId: { type: graphql.GraphQLString },
          isArchived: { type: graphql.GraphQLList(types.common.isArchived) },
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
          auth0jwt: { type: types.jwt },
          auth0accessToken: { type: graphql.GraphQLString },
          authToken: { type: graphql.GraphQLString },
        },
        resolve: resolvers.authentications.exchangeToken,
      },

      authenticated: {
        type: graphql.GraphQLNonNull(types.authentications.authenticated),
        resolve: resolvers.authentications.authenticated,
      },
    },
  }),

  mutation: new graphql.GraphQLObjectType({
    name: 'Mutation',
    fields: {

      measurementCreate: {
        type: types.measurements.measurement,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.measurements.measurementCreateInput,
            ),
          },
        },
        resolve: resolvers.measurements.create,
      },

      measurableCreate: {
        type: types.measurables.measurable,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.measurables.measurableCreateInput,
            ),
          },
        },
        resolve: resolvers.measurables.create,
      },

      seriesCreate: {
        type: types.series.series,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.series.seriesCreateInput,
            ),
          },
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

      invitationCreate: {
        type: types.invitations.invitation,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.invitations.invitationCreateInput,
            ),
          },
        },
        resolve: resolvers.invitations.create,
      },

      measurableUpdate: {
        type: types.measurables.measurable,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: {
            type: graphql.GraphQLNonNull(
              types.measurables.measurableUpdateInput,
            ),
          },
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

      userAccessTokenUpdate: {
        type: types.users.user,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: {
            type: graphql.GraphQLNonNull(
              types.users.userAccessTokenUpdateInput,
            ),
          },
        },
        resolve: resolvers.users.accessTokenUpdate,
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
          input: {
            type: graphql.GraphQLNonNull(
              types.channelMemberships.channelMembershipRoleInput,
            ),
          },
        },
        resolve: resolvers.channelMemberships.create,
      },

      channelMembershipRoleUpdate: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.channelMemberships.channelMembershipRoleInput,
            ),
          },
        },
        resolve: resolvers.channelMemberships.update,
      },

      channelMembershipDelete: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.channelMemberships.channelMembershipDeleteInput,
            ),
          },
        },
        resolve: resolvers.channelMemberships.remove,
      },

      leaveChannel: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.channelMemberships.joiningChannelInput,
            ),
          },
        },
        resolve: resolvers.channelMemberships.leave,
      },

      joinChannel: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.channelMemberships.joiningChannelInput,
            ),
          },
        },
        resolve: resolvers.channelMemberships.join,
      },

      botCreate: {
        type: types.bots.bot,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.bots.botInput,
            ),
          },
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

      notebookCreate: {
        type: types.notebooks.notebook,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.notebooks.notebookCreateInput,
            ),
          },
        },
        resolve: resolvers.notebooks.create,
      },

      notebookUpdate: {
        type: types.notebooks.notebook,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: {
            type: graphql.GraphQLNonNull(
              types.notebooks.notebookUpdateInput,
            ),
          },
        },
        resolve: resolvers.notebooks.update,
      },

      notebookDelete: {
        type: types.notebooks.notebook,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.notebookId) },
        },
        resolve: resolvers.notebooks.remove,
      },

      preferenceUpdate: {
        type: types.preferences.preference,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: {
            type: graphql.GraphQLNonNull(
              types.preferences.preferenceUpdateInput,
            ),
          },
        },
        resolve: resolvers.preferences.update,
      },

      unsubscribe: {
        type: graphql.GraphQLBoolean,
        args: {
          agentId: { type: graphql.GraphQLNonNull(types.scalars.agentId) },
        },
        resolve: resolvers.preferences.unsubscribe,
      },

      subscribe: {
        type: graphql.GraphQLBoolean,
        args: {
          agentId: { type: graphql.GraphQLNonNull(types.scalars.agentId) },
        },
        resolve: resolvers.preferences.subscribe,
      },

      globalSettingUpdate: {
        type: types.globalSettings.globalSetting,
        args: {
          name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: {
            type: graphql.GraphQLNonNull(
              types.globalSettings.globalSettingUpdateInput,
            ),
          },
        },
        resolve: resolvers.globalSettings.update,
      },

      mutexTake: {
        type: types.mutexes.mutex,
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.mutexes.mutexCreateInput,
            ),
          },
        },
        resolve: resolvers.mutexes.take,
      },

      mutexFree: {
        type: graphql.GraphQLBoolean,
        args: {
          id: {
            type: graphql.GraphQLNonNull(graphql.GraphQLString),
          },
        },
        resolve: resolvers.mutexes.free,
      },

      upvote: {
        type: graphql.GraphQLBoolean,
        args: {
          measurableId: {
            type: graphql.GraphQLNonNull(types.scalars.measurableId),
          },
        },
        resolve: resolvers.votes.upvote,
      },

      downvote: {
        type: graphql.GraphQLBoolean,
        args: {
          measurableId: {
            type: graphql.GraphQLNonNull(types.scalars.measurableId),
          },
        },
        resolve: resolvers.votes.downvote,
      },
    },
  }),
});

const schemaWithMiddlewares = applyMiddleware(
  schema,
  middlewares,
  permissions,
);

module.exports = {
  schemaWithMiddlewares,
};
