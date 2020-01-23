const graphql = require('graphql');
const { resolver } = require('graphql-sequelize');

const resolvers = require('../resolvers');
const channelMemberships = require('./channel-memberhips');
const models = require('../../models/definitions');

const { agentType } = require('./enums/agent-type');

const agent = new graphql.GraphQLObjectType({
  name: 'Agent',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    type: { type: graphql.GraphQLNonNull(agentType) },
    name: { type: graphql.GraphQLString },

    measurementCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: require('../resolvers').measurements.measurementCountByAgentId,
    },

    isMe: require('./common').isMe,
    isAdmin: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },

    // @todo: security
    // @todo: Do not use resolver. Use common interfaces of Data layer.
    user: {
      type: require('./users').user,
      resolve: resolver(models.Agent.User),
    },

    // @todo: security
    // @todo: Do not use resolver. Use common interfaces of Data layer.
    bot: {
      type: require('./bots').bot,
      resolve: resolver(models.Agent.Bot),
    },

    // @todo: security
    preference: {
      type: require('./preferences').preference,
      resolve: require('../resolvers').preferences.getOne,
    },

    // OK
    measurements: {
      type: require('./measurements').agentMeasurementsConnection,
      args: require('./common').connectionArguments,
      resolve: require('../resolvers/measurements').all,
    },

    // OK
    channels: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(require('./channels').channel),
      ),
      resolve: require('../resolvers/channels').all,
    },

    // OK
    channelMemberships: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(
        channelMemberships.channelsMembership,
      )),
      resolve: resolvers.channelMemberships.allByAgentId,
    },
  }),
});

module.exports = {
  agent,
  agentType,
};
