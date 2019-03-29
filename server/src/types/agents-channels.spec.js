const graphql = require("graphql");

const agents = require('./agents-channels');

describe('agents channels types', () => {
  it('class should be object', () => {
    expect(agents).toBeInstanceOf(Object);
  });
  it('agentsChannel', () => {
    expect(agents.agentsChannel).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
