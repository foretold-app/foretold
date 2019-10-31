const graphql = require('graphql');

const notebooks = require('./notebooks');

describe('Notebooks Types', () => {
  it('returns an object', () => {
    expect(notebooks)
      .toBeInstanceOf(Object);
  });
  it('graphql object types', () => {
    expect(notebooks.notebook)
      .toBeInstanceOf(graphql.GraphQLObjectType);
    expect(notebooks.notebookCreateInput)
      .toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
});
