const graphql = require('graphql');

const resolvers = require('../resolvers');
const channelMemberships = require('./channel-memberhips');

const { agentType } = require('./enums');
const commonTypes = require('./common');
const preferencesTypes = require('./preferences');
const usersTypes = require('./users');
const botsTypes = require('./bots');
const scalars = require('./scalars');

const agent = new graphql.GraphQLObjectType({
  name: 'Agent',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.$agentId) },
    type: { type: graphql.GraphQLNonNull(agentType) },
    name: { type: graphql.GraphQLString },

    measurementCount: {
      type: graphql.GraphQLNonNull(graphql.GraphQLInt),
      resolve: resolvers.measurements.measurementCountByAgentId,
    },

    isMe: commonTypes.isMe,
    isAdmin: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },

    // @todo: security
    user: {
      type: usersTypes.user,
      resolve: resolvers.users.oneByAgentId,
    },

    // @todo: security
    bot: {
      type: botsTypes.bot,
      resolve: resolvers.bots.oneByAgentId,
    },

    // @todo: security
    preference: {
      type: preferencesTypes.preference,
      resolve: resolvers.preferences.getOne,
    },

    measurements: {
      type: require('./measurements').agentMeasurementsConnection,
      args: commonTypes.connectionArguments,
      resolve: resolvers.measurements.all,
    },

    channels: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(require('./channels').channel),
      ),
      resolve: resolvers.channels.all,
    },

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
