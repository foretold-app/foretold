const graphql = require("graphql");

const mutations = require('./mutations');

describe('mutations types', () => {
  it('class should be object', () => {
    expect(mutations).toBeInstanceOf(Object);
  });
  it('mutations is an enum type', () => {
    expect(mutations.mutations).toBeInstanceOf(graphql.GraphQLEnumType);
  });
});
