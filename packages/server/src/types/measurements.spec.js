const graphql = require('graphql');
const measurements = require('./measurements');

describe('Measurements Types', () => {
  it('measurements is an object', () => {
    expect(measurements).toBeInstanceOf(Object);
  });
  it('measurement is an object type', () => {
    expect(measurements.measurement).toBeInstanceOf(graphql.GraphQLObjectType);
  });
  it('measurementCreateInput is an input type', () => {
    expect(measurements.measurementCreateInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
});
