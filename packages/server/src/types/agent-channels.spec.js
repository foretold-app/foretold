const graphql = require("graphql");

const agents = require('./agent-channels');

describe('Agent Channels Types', () => {
  it('class should be object', () => {
    expect(agents).toBeInstanceOf(Object);
  });
  it('returns an object type', () => {
    expect(agents.agentChannel).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
