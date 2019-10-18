const graphql = require('graphql');

const mutexes = require('./mutexes');

describe('Mutexes Types', () => {
  it('returns an object', () => {
    expect(mutexes)
      .toBeInstanceOf(Object);
  });
  it('graphql object types', () => {
    expect(mutexes.mutex)
      .toBeInstanceOf(graphql.GraphQLObjectType);
    expect(mutexes.mutexCreateInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
});
