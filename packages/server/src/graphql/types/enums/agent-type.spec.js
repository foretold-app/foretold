const graphql = require('graphql');

const { agentType } = require('./agent-type');

describe('Agent Type Enum', () => {
  it('agentType', () => {
    expect(agentType)
      .toBeInstanceOf(graphql.GraphQLEnumType);
  });
});
