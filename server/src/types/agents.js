const graphql = require("graphql");
const { attributeFields, resolver } = require("graphql-sequelize");

const resolvers = require('../resolvers');
const channelsMemberships = require('./channels-memberhips');
const models = require("../models");

const agentType = new graphql.GraphQLEnumType({
  name: 'AgentType',
  values: {
    USER: {
      value: 'USER',
    },
    BOT: {
      value: 'BOT',
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
    channelMemberships: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(channelsMemberships.channelsMembership)),
      resolve: resolvers.channelsMemberships.allByAgentId,
    },
    User: {
      type: require('./').userType,
      resolve: resolver(models.User)
    },
    Bot: {
      type: require('./').botType,
      resolve: resolver(models.Bot)
    },
    Measurements: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(require('./').measurementType)),
      resolve: resolver(models.Measurement)
    },
    Measurables: {
      type: graphql.GraphQLNonNull(require('./').measurableType),
      resolve: resolver(models.Measurable)
    },
    Channels: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(require('./').channels.channel)),
      resolve: resolver(models.Channel)
    }
  })
});

module.exports = {
  agent,
  agentType,
};
