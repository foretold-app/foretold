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
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
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
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
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
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolver(models.Bot),
      },

      bots: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.botType)),
        resolve: resolver(models.Bot),
      },

      agent: {
        type: types.agents.agent,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
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
        args: {
          input: { type: types.measurements.measurementCreateInput },
        },
        resolve: resolvers.measurements.create,
      },

      measurableCreate: {
        type: types.measurables.measurable,
        args: {
          input: { type: types.measurables.measurableCreateInput }
        },
        resolve: resolvers.measurables.create,
      },

      seriesCreate: {
        type: types.seriesType,
        args: {
          input: {
            type: new graphql.GraphQLInputObjectType({
              name: 'SeriesCreateInput',
              fields: {
                name: { type: graphql.GraphQLString },
                description: { type: graphql.GraphQLString },
                channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
                subjects: { type: graphql.GraphQLList(graphql.GraphQLString) },
                properties: { type: graphql.GraphQLList(graphql.GraphQLString) },
                dates: { type: graphql.GraphQLList(require('graphql-sequelize').DateType.default) },
              }
            })
          }
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
          input: {
            type: new graphql.GraphQLInputObjectType({
              name: 'MeasurableUpdateInput',
              fields: {
                name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
                labelCustom: { type: graphql.GraphQLString },
                expectedResolutionDate: { type: require('graphql-sequelize').DateType.default },
                resolutionEndpoint: { type: graphql.GraphQLString },
                labelSubject: { type: graphql.GraphQLString },
                labelOnDate: { type: require('graphql-sequelize').DateType.default },
                labelProperty: { type: graphql.GraphQLString },
              }
            })
          }
        },
        resolve: resolvers.measurables.update,
      },

      userUpdate: {
        type: types.userType,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
          input: {
            type: new graphql.GraphQLInputObjectType({
              name: 'UserUpdateInput',
              fields: {
                name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
              }
            })
          }
        },
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
          input: {
            type: new graphql.GraphQLInputObjectType({
              name: 'ChannelMembershipRoleCreateInput',
              fields: {
                agentId: { type: graphql.GraphQLString },
                channelId: { type: graphql.GraphQLString },
                role: { type: graphql.GraphQLNonNull(types.channelMemberships.role) },
              }
            })
          }
        },
        resolve: resolvers.channelMemberships.create,
      },

      channelMembershipRoleUpdate: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: {
            type: new graphql.GraphQLInputObjectType({
              name: 'ChannelMembershipRoleUpdateInput',
              fields: {
                agentId: { type: graphql.GraphQLString },
                channelId: { type: graphql.GraphQLString },
                role: { type: graphql.GraphQLNonNull(types.channelMemberships.role) },
              }
            })
          }
        },
        resolve: resolvers.channelMemberships.update,
      },

      channelMembershipDelete: {
        type: types.channelMemberships.channelsMembership,
        args: {
          input: {
            type: new graphql.GraphQLInputObjectType({
              name: 'ChannelMembershipDeleteInput',
              fields: {
                agentId: { type: graphql.GraphQLString },
                channelId: { type: graphql.GraphQLString },
              }
            })
          }
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
