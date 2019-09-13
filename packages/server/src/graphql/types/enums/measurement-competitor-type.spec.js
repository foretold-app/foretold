const graphql = require('graphql');

const competitors = require('./measurement-competitor-type');

describe('competitors types', () => {
  it('class should be object', () => {
    expect(competitors).toBeInstanceOf(Object);
  });
  it('channel', () => {
    expect(competitors.measurementCompetitorType).toBeInstanceOf(graphql.GraphQLNonNull);
  });
});
