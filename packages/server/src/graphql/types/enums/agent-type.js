const graphql = require('graphql');

const { AGENT_TYPE } = require('../../../enums');

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

module.exports = {
  agentType,
};
