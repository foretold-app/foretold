const { ChannelModel } = require('./channel-model');

describe('Channel Model', () => {
  it('class should be a constructor', () => {
    expect(ChannelModel).toBeInstanceOf(Function);
  });
});
