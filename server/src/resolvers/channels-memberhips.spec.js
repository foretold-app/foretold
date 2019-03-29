const channelsMemberships = require('./channels-memberhips');
const data = require('../data');

describe('ChannelsMemberships Resolvers', () => {

  describe('create()', () => {
    const root = {};
    const args = {
      channelId: 'channelId1',
      agentId: 'agentId2',
      role: 'ADMIN'
    };
    beforeEach(() => {
      jest.spyOn(data.channelsMembershipsData, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates agent-channel row', () => {
      return channelsMemberships.create(root, args).then((result) => {
        expect(data.channelsMembershipsData.createOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
          args.role,
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
      role: ['admin']
    };
    beforeEach(() => {
      jest.spyOn(data.channelsMembershipsData, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('updates agent-channel row', () => {
      return channelsMemberships.create(root, args).then((result) => {
        expect(data.channelsMembershipsData.createOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
          args.role,
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
      jest.spyOn(data.channelsMembershipsData, 'deleteOne')
        .mockReturnValue(Promise.resolve(true));
    });
    it('removes agent-channel row', () => {
      return channelsMemberships.remove(root, args).then((result) => {
        expect(data.channelsMembershipsData.deleteOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
        );
        expect(result).toBe(true);
      });
    });
  });

});
