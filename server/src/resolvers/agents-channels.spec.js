const agentsChannels = require('./agents-channels');
const data = require('../data');

describe('AgentsChannels Resolvers', () => {

  describe('create()', () => {
    const root = {};
    const args = { channelId: 'channelId1', agentId: 'agentId2' };
    beforeEach(() => {
      jest.spyOn(data.agentsChannelsData, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates agent-channel row', () => {
      return agentsChannels.create(root, args).then((result) => {
        expect(data.agentsChannelsData.createOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('remove()', () => {
    const root = {};
    const args = { channelId: 'channelId2', agentId: 'agentId3' };
    beforeEach(() => {
      jest.spyOn(data.agentsChannelsData, 'deleteOne')
        .mockReturnValue(Promise.resolve(true));
    });
    it('removes agent-channel row', () => {
      return agentsChannels.remove(root, args).then((result) => {
        expect(data.agentsChannelsData.deleteOne).toHaveBeenCalledWith(
          'channelId2',
          'agentId3',
        );
        expect(result).toBe(true);
      });
    });
  });

});
