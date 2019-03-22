const graphql = require("graphql");

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

const agent2 = new graphql.GraphQLObjectType({
  name: 'Agent2',
  fields: {
    id: { type: graphql.GraphQLString },
    type: { type: graphql.GraphQLNonNull(agentType) },
    name: { type: graphql.GraphQLString },
    measurementCount: { type: graphql.GraphQLInt },
  }
});

module.exports = {
  agent2,
  agentType,
};
