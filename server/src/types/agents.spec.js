const graphql = require("graphql");

const agents = require('./agents');

describe('agents types', () => {
  it('class should be object', () => {
    expect(agents).toBeInstanceOf(Object);
  });
  it('agentType', () => {
    expect(agents.agentType).toBeInstanceOf(graphql.GraphQLEnumType);
  });
  it('agent2', () => {
    expect(agents.agent).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
