const graphql = require("graphql");

const authentications = require('./authentications');

describe('authentications types', () => {
  it('class should be object', () => {
    expect(authentications).toBeInstanceOf(Object);
  });
  it('authenticationJwt is an object type', () => {
    expect(authentications.authenticationJwt).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
