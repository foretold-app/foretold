const graphql = require("graphql");
const { resolver } = require("graphql-sequelize");

const resolvers = require('../resolvers');
const channelMemberships = require('./channel-memberhips');
const models = require("../models");

const { AGENT_TYPE } = require('../models/enums/agent-type');

const agentType = new graphql.GraphQLEnumType({
  name: 'AgentType',
  values: {
    [AGENT_TYPE.USER]: {
      value: AGENT_TYPE.USER,
    },
    [AGENT_TYPE.BOT]: {
      value: AGENT_TYPE.BOT,
    },
  },
});

const agent = new graphql.GraphQLObjectType({
  name: 'Agent',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    type: { type: graphql.GraphQLNonNull(agentType) },
    name: { type: graphql.GraphQLString },
    measurementCount: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    isMe: require('./common').isMe,
    isAdmin: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },

    // @todo: security
    User: {
      type: require('./users').user,
      resolve: resolver(models.Agent.User),
    },

    // @todo: security
    Bot: {
      type: require('./bots').bot,
      resolve: resolver(models.Agent.Bot),
    },

    // @todo: security
    Preference: {
      type: require('./preferences').preference,
      resolve: require('../resolvers').preferences.getOne,
    },

    // OK
    Measurements: {
      type: require('./measurements').agentMeasurementsConnection,
      args: require('./common').connectionArguments,
      resolve: require('../resolvers/measurements').all,
    },

    // OK
    Channels: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(require('./channels').channel)),
      resolve: require('../resolvers/channels').all,
    },

    // OK
    channelMemberships: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(channelMemberships.channelsMembership)),
      resolve: resolvers.channelMemberships.allByAgentId,
    },
  })
});

module.exports = {
  agent,
  agentType,
};
