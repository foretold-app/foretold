const graphql = require('graphql');
const series = require('./series');

describe('Series Types', () => {
  it('series is an object', () => {
    expect(series).toBeInstanceOf(Object);
  });
  it('series is an object type', () => {
    expect(series.series).toBeInstanceOf(graphql.GraphQLObjectType);
  });
  it('seriesCreateInput is an input type', () => {
    expect(series.seriesCreateInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
});
