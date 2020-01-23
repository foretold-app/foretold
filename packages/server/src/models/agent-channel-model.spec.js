const { AgentChannelModel } = require('./agent-channel-model');

describe('Agent Channel Model', () => {
  it('class should be a constructor', () => {
    expect(AgentChannelModel).toBeInstanceOf(Function);
  });
});
