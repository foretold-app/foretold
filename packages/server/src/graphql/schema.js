const graphql = require('graphql');
const { applyMiddleware } = require('graphql-middleware');

const resolvers = require('./resolvers');

const types = require('./types');
const { stats } = require('./types/stats');

const { permissions } = require('./authorizers');
const { middlewares } = require('./middlewares');
const { validators } = require('./validators');

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
          channelId: { type: types.scalars.$channelId },
          measurableId: { type: types.scalars.$measurableId },
        },
        resolve: resolvers.permissions.all,
      },

      user: {
        type: types.users.user,
        args: {
          id: { type: types.scalars.$userId },
        },
        resolve: resolvers.users.one,
      },

      users: {
        type: graphql.GraphQLNonNull(graphql.GraphQLList(types.users.user)),
        resolve: resolvers.users.all,
      },

      measurement: {
        type: types.measurements.measurement,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$measurementId) },
        },
        resolve: resolvers.measurements.one,
      },

      measurements: {
        type: types.measurements.measurementsConnection,
        args: {
          ...types.common.connectionArguments,

          // IDs
          measurableId: { type: types.scalars.$measurableId },
          agentId: { type: types.scalars.$agentId },
          channelId: { type: types.scalars.$channelId },
          notTaggedByAgent: { type: types.scalars.$agentId },

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
          isVerified: { type: graphql.GraphQLBoolean },
        },
        resolve: resolvers.measurements.all,
      },

      measurable: {
        type: types.measurables.measurable,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$measurableId) },
        },
        resolve: resolvers.measurables.one,
      },

      measurables: {
        type: types.measurables.measurablesConnection,
        args: {
          ...types.common.connectionArguments,
          measurableIds: {
            type: graphql.GraphQLList(
              graphql.GraphQLNonNull(types.scalars.$measurableId),
            ),
          },
          creatorId: { type: types.scalars.$agentId },
          seriesId: { type: types.scalars.$seriesId },
          channelId: { type: types.scalars.$channelId },
          measuredByAgentId: { type: types.scalars.$agentId },
          states: {
            type: graphql.GraphQLList(
              types.measurables.measurableState,
            ),
          },
          isArchived: { type: graphql.GraphQLList(types.common.isArchived) },
          order: {
            type: graphql.GraphQLList(
              types.measurables.orderMeasurables,
            ),
          },
          labelProperty: {
            type: graphql.GraphQLList(
              graphql.GraphQLNonNull(graphql.GraphQLString),
            ),
          },
          labelSubject: {
            type: graphql.GraphQLList(
              graphql.GraphQLNonNull(graphql.GraphQLString),
            ),
          },
        },
        resolve: resolvers.measurables.all,
      },

      agentMeasurables: {
        type: types.agentMeasurables.agentMeasurablesConnection,
        args: {
          ...types.common.connectionArguments,
          channelId: { type: types.scalars.$channelId },
          measurableId: { type: types.scalars.$measurableId },
          measurableState: {
            type: graphql.GraphQLList(
              types.measurables.measurableState,
            ),
          },
          minPredictionCountTotal: { type: graphql.GraphQLInt },
          order: {
            type: graphql.GraphQLList(
              types.agentMeasurables.orderAgentMeasurables,
            ),
          },
        },
        resolve: resolvers.agentMeasurables.all,
      },

      agentChannels: {
        type: types.agentChannels.agentChannelsConnection,
        args: {
          ...types.common.connectionArguments,
          minNumberOfPredictions: { type: graphql.GraphQLInt },
          minNumberOfQuestionsScored: { type: graphql.GraphQLInt },
          channelId: { type: types.scalars.$channelId },
          agentId: { type: types.scalars.$agentId },
        },
        resolve: resolvers.agentChannels.all,
      },

      bot: {
        type: types.bots.bot,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$botId) },
        },
        resolve: resolvers.bots.one,
      },

      bots: {
        type: types.bots.botsConnection,
        args: {
          ...types.common.connectionArguments,
          ownerId: { type: types.scalars.$agentId },
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
          order: { type: graphql.GraphQLList(types.notebooks.orderNotebooks) },
        },
        resolve: resolvers.notebooks.all,
      },

      feedItems: {
        type: types.feedItems.feedItemsConnection,
        args: {
          ...types.common.connectionArguments,
          channelId: { type: types.scalars.$channelId },
          agentId: { type: types.scalars.$agentId },
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
          id: { type: graphql.GraphQLNonNull(types.scalars.$agentId) },
        },
        resolve: resolvers.agents.one,
      },

      agents: {
        args: {
          excludeChannelId: { type: types.scalars.$channelId },
          types: { type: graphql.GraphQLList(types.agents.agentType) },
        },
        type: graphql.GraphQLNonNull(graphql.GraphQLList(types.agents.agent)),
        resolve: resolvers.agents.all,
      },

      series: {
        type: types.series.series,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$seriesId) },
        },
        resolve: resolvers.series.one,
      },

      seriesCollection: {
        type: graphql.GraphQLNonNull(graphql.GraphQLList(types.series.series)),
        args: {
          channelId: { type: types.scalars.$channelId },
        },
        resolve: resolvers.series.all,
      },

      channel: {
        type: types.channels.channel,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$channelId) },
        },
        resolve: resolvers.channels.one,
      },

      channels: {
        type: graphql.GraphQLNonNull(graphql.GraphQLList(
          types.channels.channel,
        )),
        args: {
          offset: { type: graphql.GraphQLInt },
          limit: { type: graphql.GraphQLInt },
          channelMemberId: { type: types.scalars.$agentId },
          isArchived: { type: graphql.GraphQLList(types.common.isArchived) },
          order: { type: graphql.GraphQLList(types.channels.orderChannels) },
        },
        resolve: resolvers.channels.all,
      },

      votes: {
        type: graphql.GraphQLNonNull(graphql.GraphQLList(
          types.votes.vote,
        )),
        args: {
          measurementId: { type: types.scalars.measurementId },
        },
        resolve: resolvers.votes.all,
      },

      stats: {
        type: graphql.GraphQLNonNull(stats),
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

      seriesUpdate: {
        type: types.series.series,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$seriesId) },
          input: {
            type: graphql.GraphQLNonNull(
              types.series.seriesUpdateInput,
            ),
          },
        },
        resolve: resolvers.series.update,
      },

      measurableArchive: {
        type: types.measurables.measurable,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$measurableId) },
        },
        resolve: resolvers.measurables.archive,
      },

      measurableUnarchive: {
        type: types.measurables.measurable,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$measurableId) },
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
          id: { type: graphql.GraphQLNonNull(types.scalars.$measurableId) },
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
          id: { type: graphql.GraphQLNonNull(types.scalars.$userId) },
          input: { type: graphql.GraphQLNonNull(types.users.userUpdateInput) },
        },
        resolve: resolvers.users.update,
      },

      userAccessTokenUpdate: {
        type: types.users.user,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$userId) },
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
          id: { type: graphql.GraphQLNonNull(types.scalars.$channelId) },
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

      channelMembershipVerify: {
        type: graphql.GraphQLNonNull(graphql.GraphQLBoolean),
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.channelMemberships.channelMembershipVerifyInput,
            ),
          },
        },
        resolve: resolvers.channelMemberships.verify,
      },

      channelMembershipUnverify: {
        type: graphql.GraphQLNonNull(graphql.GraphQLBoolean),
        args: {
          input: {
            type: graphql.GraphQLNonNull(
              types.channelMemberships.channelMembershipUnverifyInput,
            ),
          },
        },
        resolve: resolvers.channelMemberships.unverify,
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
          id: { type: graphql.GraphQLNonNull(types.scalars.$botId) },
          input: { type: graphql.GraphQLNonNull(types.bots.botInput) },
        },
        resolve: resolvers.bots.update,
      },

      botTokenRefresh: {
        type: types.authentications.authenticationToken,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$botId) },
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
          id: { type: graphql.GraphQLNonNull(types.scalars.$notebookId) },
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

      channelBookmarkToggle: {
        type: graphql.GraphQLBoolean,
        args: {
          channelId: {
            type: graphql.GraphQLNonNull(
              types.scalars.channelId,
            ),
          },
        },
        resolve: resolvers.channelBookmarks.toggle,
      },

      notebookBookmarkToggle: {
        type: graphql.GraphQLBoolean,
        args: {
          notebookId: {
            type: graphql.GraphQLNonNull(
              types.scalars.notebookId,
            ),
          },
        },
        resolve: resolvers.notebookBookmarks.toggle,
      },

      preferenceUpdate: {
        type: types.preferences.preference,
        args: {
          id: { type: graphql.GraphQLNonNull(types.scalars.$preferenceId) },
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
            type: graphql.GraphQLNonNull(types.scalars.$mutexId),
          },
        },
        resolve: resolvers.mutexes.free,
      },

      measurementVote: {
        type: types.votes.vote,
        args: {
          measurementId: {
            type: graphql.GraphQLNonNull(
              types.scalars.measurementId,
            ),
          },
          input: {
            type: graphql.GraphQLNonNull(
              types.votes.voteInput,
            ),
          },
        },
        resolve: resolvers.votes.measurementVote,
      },
    },
  }),
});

const schemaWithMiddlewares = applyMiddleware(
  schema,
  middlewares,
  validators,
  permissions,
);

module.exports = {
  schemaWithMiddlewares,
};
