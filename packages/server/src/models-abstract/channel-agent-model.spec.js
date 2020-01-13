const { ChannelAgentModel } = require('./channel-agent-model');

describe('Channel Agent Model', () => {
  it('class should be a constructor', () => {
    expect(ChannelAgentModel).toBeInstanceOf(Function);
  });
});
