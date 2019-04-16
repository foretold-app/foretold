const graphql = require("graphql");

const authentications = require('./authentications');

describe('authentications types', () => {
  it('class should be object', () => {
    expect(authentications).toBeInstanceOf(Object);
  });
  it('authentication is an enum type', () => {
    expect(authentications.authentication).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
