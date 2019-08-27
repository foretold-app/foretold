const data = require('./agent-channels-data');

describe('Agent Channels Data', () => {
  it('class should be a constructor', () => {
    expect(data.AgentChannelsData).toBeInstanceOf(Function);
  });
});
