const { ChannelsData } = require('./channels-data');

describe('Channels Data Layer', () => {
  it('class should be constructor', () => {
    expect(ChannelsData).toBeInstanceOf(Function);
  });
});
