const agentsChannels = require('./agents-channels');
const data = require('../data');

describe('AgentsChannels Resolvers', () => {

  describe('create()', () => {
    const root = {};
    const args = {
      channelId: 'channelId1',
      agentId: 'agentId2',
      roles: ['admin']
    };
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
          args.roles,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('update()', () => {
    const root = {};
    const args = {
      channelId: 'channelId1',
      agentId: 'agentId2',
      roles: ['admin']
    };
    beforeEach(() => {
      jest.spyOn(data.agentsChannelsData, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('updates agent-channel row', () => {
      return agentsChannels.create(root, args).then((result) => {
        expect(data.agentsChannelsData.createOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
          args.roles,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('remove()', () => {
    const root = {};
    const args = {
      channelId: 'channelId1',
      agentId: 'agentId2',
    };
    beforeEach(() => {
      jest.spyOn(data.agentsChannelsData, 'deleteOne')
        .mockReturnValue(Promise.resolve(true));
    });
    it('removes agent-channel row', () => {
      return agentsChannels.remove(root, args).then((result) => {
        expect(data.agentsChannelsData.deleteOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
        );
        expect(result).toBe(true);
      });
    });
  });

});
