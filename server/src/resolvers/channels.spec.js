const channels = require('./channels');
const data = require('../data');

describe('channels', () => {
  afterEach(() => {
    jest.clearAllMocks();
    jest.restoreAllMocks();
  });

  it('class should be constructor', () => {
    expect(channels.channelAgents).toBeInstanceOf(Function);
    expect(channels.channelCreator).toBeInstanceOf(Function);
  });

  describe('channelAgents', () => {
    const channel = {id: '1'};
    beforeAll(() => {
      jest.spyOn(data.channelsData, 'getAgentsByChannelId').mockReturnValue(Promise.resolve(true));
    });
    it('channelAgents', () => {
      return channels.channelAgents(channel).then((result) => {
        expect(data.channelsData.getAgentsByChannelId).toHaveBeenCalledWith(channel.id);
        expect(result).toBe(true);
      });
    });
  });

  describe('channelCreator', () => {
    const channel = {id: '1'};
    beforeAll(() => {
      jest.spyOn(data.channelsData, 'getCreatorByChannelId').mockReturnValue(Promise.resolve(true));
    });
    it('channelCreator', () => {
      return channels.channelCreator(channel).then((result) => {
        expect(data.channelsData.getCreatorByChannelId).toHaveBeenCalledWith(channel.id);
        expect(result).toBe(true);
      });
    });
  });

});
