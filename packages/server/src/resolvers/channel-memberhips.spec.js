const channelMemberships = require('./channel-memberhips');
const data = require('../data');

describe('ChannelMemberships Resolvers', () => {

  describe('create()', () => {
    const root = {};
    const args = {
      input: {
        channelId: 'channelId1',
        agentId: 'agentId2',
        role: 'ADMIN'
      },
    };
    beforeEach(() => {
      jest.spyOn(data.channelMemberships, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates agent-channel row', () => {
      return channelMemberships.create(root, args).then((result) => {
        expect(data.channelMemberships.createOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
          args.input.role,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('update()', () => {
    const root = {};
    const args = {
      input: {
        channelId: 'channelId1',
        agentId: 'agentId2',
        role: ['admin']
      }
    };
    beforeEach(() => {
      jest.spyOn(data.channelMemberships, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('updates agent-channel row', () => {
      return channelMemberships.create(root, args).then((result) => {
        expect(data.channelMemberships.createOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
          args.input.role,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('remove()', () => {
    const root = {};
    const args = {
      input: {
        channelId: 'channelId1',
        agentId: 'agentId2',
      }
    };
    beforeEach(() => {
      jest.spyOn(data.channelMemberships, 'deleteOne')
        .mockReturnValue(Promise.resolve(true));
    });
    it('removes agent-channel row', () => {
      return channelMemberships.remove(root, args).then((result) => {
        expect(data.channelMemberships.deleteOne).toHaveBeenCalledWith(
          'channelId1',
          'agentId2',
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('myRole()', () => {
    const root = { id: 'id1' };
    const args = {};
    const context = { agent: { id: 'agentId10' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channelMemberships, 'getOneOnlyRole')
        .mockReturnValue(Promise.resolve(true));
    });
    it('returns only role string', () => {
      return channelMemberships.myRole(root, args, context, info)
        .then((result) => {
          expect(data.channelMemberships.getOneOnlyRole).toHaveBeenCalledWith({
            "agentId": "agentId10",
            "channelId": "id1"
          });
          expect(result).toBe(true);
        });
    });
  });

  describe('leave()', () => {
    const root = {};
    const args = { input: { channelId: 'channelId1' } };
    const context = { agent: { id: 'agentId11' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channelMemberships, 'leave')
        .mockReturnValue(Promise.resolve(true));
    });
    it('leaves channel', () => {
      return channelMemberships.leave(root, args, context, info)
        .then((result) => {
          expect(data.channelMemberships.leave).toHaveBeenCalledWith({
            "agentId": "agentId11",
            "channelId": "channelId1"
          });
          expect(result).toBe(true);
        });
    });
  });

  describe('join()', () => {
    const root = {};
    const args = { input: { channelId: 'channelId2' } };
    const context = { agent: { id: 'agentId12' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channelMemberships, 'join')
        .mockReturnValue(Promise.resolve(true));
    });
    it('joins channel', () => {
      return channelMemberships.join(root, args, context, info)
        .then((result) => {
          expect(data.channelMemberships.join).toHaveBeenCalledWith({
            "agentId": "agentId12",
            "channelId": "channelId2"
          });
          expect(result).toBe(true);
        });
    });
  });

});
