const channelMemberships = require('./channel-memberhips');
const data = require('../data');

describe('ChannelMemberships Resolvers', () => {

  describe('create()', () => {
    const root = {};
    const args = {
      channelId: 'channelId1',
      agentId: 'agentId2',
      role: 'ADMIN'
    };
    beforeEach(() => {
      jest.spyOn(data.channelMembershipsData, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates agent-channel row', () => {
      return channelMemberships.create(root, args).then((result) => {
        expect(data.channelMembershipsData.createOne).toHaveBeenCalledWith(
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
      jest.spyOn(data.channelMembershipsData, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('updates agent-channel row', () => {
      return channelMemberships.create(root, args).then((result) => {
        expect(data.channelMembershipsData.createOne).toHaveBeenCalledWith(
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
      jest.spyOn(data.channelMembershipsData, 'deleteOne')
        .mockReturnValue(Promise.resolve(true));
    });
    it('removes agent-channel row', () => {
      return channelMemberships.remove(root, args).then((result) => {
        expect(data.channelMembershipsData.deleteOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
        );
        expect(result).toBe(true);
      });
    });
  });

});
