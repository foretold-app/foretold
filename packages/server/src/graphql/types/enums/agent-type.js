const graphql = require('graphql');

const { AGENT_TYPE } = require('../../../enums/agent-type');

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
