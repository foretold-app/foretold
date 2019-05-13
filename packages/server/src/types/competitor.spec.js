const graphql = require("graphql");

const competitors = require('./competitor');

describe('competitors types', () => {
  it('class should be object', () => {
    expect(competitors).toBeInstanceOf(Object);
  });
  it('channel', () => {
    expect(competitors.competitor).toBeInstanceOf(graphql.GraphQLNonNull);
  });
});
