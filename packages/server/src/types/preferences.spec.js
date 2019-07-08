const graphql = require('graphql');

const preferences = require('./preferences');

describe('Preference Types', () => {
  it('preferences is an object', () => {
    expect(preferences).toBeInstanceOf(Object);
  });
  it('preference type is an object type', () => {
    expect(preferences.preference).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
