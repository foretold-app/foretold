const data = require('./channel-agents-data');

describe('Channel Agents Data', () => {
  it('class should be a constructor', () => {
    expect(data.AgentChannelsData).toBeInstanceOf(Function);
  });
});
