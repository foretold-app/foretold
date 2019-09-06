const graphql = require('graphql');

const bots = require('./bots');

describe('Bots Types', () => {
  it('bots is an object', () => {
    expect(bots).toBeInstanceOf(Object);
  });
  it('bot type is an object type', () => {
    expect(bots.bot).toBeInstanceOf(graphql.GraphQLObjectType);
  });
});
