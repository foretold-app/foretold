const graphql = require("graphql");

const agents = require('./agent-measurables');

describe('Agents Measurables Types', () => {
  it('class should be object', () => {
    expect(agents).toBeInstanceOf(Object);
  });
  it('agentMeasurable is an object type', () => {
    expect(agents.agentMeasurable).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
