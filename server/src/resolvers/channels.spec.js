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
    const channel = { id: '1' };
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
    const channel = { id: '1' };
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

  describe('all', () => {
    const root = {};
    const context = { user: { agentId: '1' } };
    const args = { offset: 1, limit: 2 };
    const info = {};
    beforeAll(() => {
      jest.spyOn(data.agentsChannelsData, 'getAllChannelIds').mockReturnValue(Promise.resolve(['3']));
      jest.spyOn(data.channelsData, 'getAll').mockReturnValue(Promise.resolve(true));
    });
    it('all', () => {
      return channels.all(root, args, context, info).then((result) => {
        expect(data.agentsChannelsData.getAllChannelIds).toHaveBeenCalledWith({ agentId: '1' });
        expect(data.channelsData.getAll).toHaveBeenCalledWith({
          "limit": 2,
          "offset": 1,
          "restrictions": { "channelIds": ["3"] }
        });
        expect(result).toBe(true);
      });
    });
  });

});
