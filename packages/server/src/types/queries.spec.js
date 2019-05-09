const graphql = require("graphql");

const queries = require('./queries');

describe('queries types', () => {
  it('class should be object', () => {
    expect(queries).toBeInstanceOf(Object);
  });
  it('queries is an enum type', () => {
    expect(queries.queries).toBeInstanceOf(graphql.GraphQLEnumType);
  });
});
