const graphql = require("graphql");

const agents = require('./agents');

describe('agents types', () => {
  it('class should be object', () => {
    expect(agents).toBeInstanceOf(Object);
  });
  it('agentType is an enum type', () => {
    expect(agents.agentType).toBeInstanceOf(graphql.GraphQLEnumType);
  });
  it('agent is an object type', () => {
    expect(agents.agent).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
