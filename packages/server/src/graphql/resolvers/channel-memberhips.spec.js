const channelMemberships = require('./channel-memberhips');
const { ChannelMembershipsData } = require('../../data');

describe('Channel Memberships Resolvers', () => {
  describe('create()', () => {
    const root = {};
    const args = {
      input: {
        channelId: 'channelId1',
        agentId: 'agentId2',
        role: 'ADMIN',
      },
    };
    const context = { agent: { id: 'agentId' } };

    it('creates agent-channel row', () => {
      return channelMemberships.create(root, args, context)
        .then((result) => {
          expect(new ChannelMembershipsData().upsertOne)
            .toHaveBeenCalledWith(
              {
                agentId: 'agentId2',
                channelId: 'channelId1',
              }, {}, {
                agentId: 'agentId2',
                channelId: 'channelId1',
                inviterAgentId: 'agentId',
                role: 'ADMIN',
              },
            );
          expect(result)
            .toBe(true);
        });
    });
  });

  describe('update()', () => {
    const root = {};
    const args = {
      input: {
        channelId: 'channelId1',
        agentId: 'agentId2',
        role: 'ADMIN',
      },
    };

    it('updates agent-channel row', () => {
      return channelMemberships.update(root, args)
        .then((result) => {
          expect(new ChannelMembershipsData().updateOne)
            .toHaveBeenCalledWith(
              {
                agentId: 'agentId2',
                channelId: 'channelId1',
              }, {
                agentId: 'agentId2',
                channelId: 'channelId1',
                role: 'ADMIN',
              },
            );
          expect(result)
            .toBe(true);
        });
    });
  });

  describe('remove()', () => {
    const root = {};
    const args = {
      input: {
        channelId: 'channelId1',
        agentId: 'agentId2',
      },
    };

    it('removes agent-channel row', () => {
      return channelMemberships.remove(root, args)
        .then((result) => {
          expect(new ChannelMembershipsData().deleteOne)
            .toHaveBeenCalledWith(
              {
                agentId: 'agentId2',
                channelId: 'channelId1',
              },
            );
          expect(result)
            .toBe(true);
        });
    });
  });

  describe('myRole()', () => {
    const root = { id: 'id1' };
    const args = {};
    const context = { agent: { id: 'agentId10' } };
    const info = {};

    it('returns only role string', () => {
      return channelMemberships.myRole(root, args, context, info)
        .then((result) => {
          expect(new ChannelMembershipsData().getOneOnlyRole)
            .toHaveBeenCalledWith({
              agentId: 'agentId10',
              channelId: 'id1',
            });
          expect(result)
            .toBe(true);
        });
    });
  });

  describe('leave()', () => {
    const root = {};
    const args = { input: { channelId: 'channelId1' } };
    const context = { agent: { id: 'agentId11' } };
    const info = {};

    it('leaves channel', () => {
      return channelMemberships.leave(root, args, context, info)
        .then((result) => {
          expect(new ChannelMembershipsData().leave)
            .toHaveBeenCalledWith({
              agentId: 'agentId11',
              channelId: 'channelId1',
            });
          expect(result)
            .toBe(true);
        });
    });
  });

  describe('join()', () => {
    const root = {};
    const args = { input: { channelId: 'channelId2' } };
    const context = { agent: { id: 'agentId12' } };
    const info = {};

    it('joins channel', () => {
      return channelMemberships.join(root, args, context, info)
        .then((result) => {
          expect(new ChannelMembershipsData().join)
            .toHaveBeenCalledWith({
              agentId: 'agentId12',
              channelId: 'channelId2',
            });
          expect(result)
            .toBe(true);
        });
    });
  });
});
