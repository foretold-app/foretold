const graphql = require("graphql");

const stats = require('./stats');

describe('stats types', () => {
  it('class should be object', () => {
    expect(stats).toBeInstanceOf(Object);
  });
  it('stats', () => {
    expect(stats.stats).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
