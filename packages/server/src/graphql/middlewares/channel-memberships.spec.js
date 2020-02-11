const { setContextChannelMemberships } = require('./channel-memberships');
const { ChannelMembershipsData } = require('../../data');

describe('Channel Memberships Middlewares', () => {
  describe('setContextChannelMemberships() sets agent-channel model into context', () => {
    const channelMembership = true;

    it('when arguments are passed', () => {
      const root = {};
      const args = { channelId: 'channelId1' };
      const context = { agent: { id: 'agentId1' } };
      const info = {};
      return setContextChannelMemberships(root, args, context, info)
        .then((result) => {
          expect(new ChannelMembershipsData().getOne).toHaveBeenCalledWith({
            agentId: 'agentId1',
            channelId: 'channelId1',
          }, {}, {
            raw: true
          });
          expect(result).toBe(undefined);
          expect(context.channelMembership).toBe(channelMembership);
        });
    });

    it('when root is passed', () => {
      const root = { channelId: 'channelId1' };
      const args = {};
      const context = { agent: { id: 'agentId2' } };
      const info = {};
      return setContextChannelMemberships(root, args, context, info)
        .then((result) => {
          expect(new ChannelMembershipsData().getOne).toHaveBeenCalledWith({
            agentId: 'agentId2',
            channelId: 'channelId1',
          }, {}, {
            raw: true
          });
          expect(result).toBe(undefined);
          expect(context.channelMembership).toBe(channelMembership);
        });
    });

    it('when context is passed', () => {
      const root = {};
      const args = {};
      const context = {
        agent: { id: 'agentId3' },
        channelId: 'channelId1',
      };
      const info = {};
      return setContextChannelMemberships(root, args, context, info)
        .then((result) => {
          expect(new ChannelMembershipsData().getOne).toHaveBeenCalledWith({
            agentId: 'agentId3',
            channelId: 'channelId1',
          }, {}, {
            raw: true
          });
          expect(result).toBe(undefined);
          expect(context.channelMembership).toBe(channelMembership);
        });
    });
  });
});
