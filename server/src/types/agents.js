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

// Used to remove circular dependencies in generateReferences function
const agent2 = new graphql.GraphQLObjectType({
  name: 'Agent2',
  fields: {
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    type: { type: graphql.GraphQLNonNull(agentType) },
    name: { type: graphql.GraphQLString },
    measurementCount: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
  }
});

module.exports = {
  agent2,
  agentType,
};
