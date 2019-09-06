const graphql = require('graphql');
const users = require('./users');

describe('Users Types', () => {
  it('users is an object', () => {
    expect(users).toBeInstanceOf(Object);
  });
  it('user is an object type', () => {
    expect(users.user).toBeInstanceOf(graphql.GraphQLObjectType);
  });
  it('userUpdateInput is an input type', () => {
    expect(users.userUpdateInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
});
