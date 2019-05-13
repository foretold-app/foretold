const graphql = require("graphql");

const channels = require('./channels');

describe('channels types', () => {
  it('class should be object', () => {
    expect(channels).toBeInstanceOf(Object);
  });
  it('channel', () => {
    expect(channels.channel).toBeInstanceOf(graphql.GraphQLObjectType);
  });
  it('channelInput', () => {
    expect(channels.channelInput).toBeInstanceOf(graphql.GraphQLInputObjectType);
  });
});
