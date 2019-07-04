const graphql = require("graphql");

const competitors = require('./competitor-type');

describe('competitors types', () => {
  it('class should be object', () => {
    expect(competitors).toBeInstanceOf(Object);
  });
  it('channel', () => {
    expect(competitors.competitorType).toBeInstanceOf(graphql.GraphQLNonNull);
  });
});
