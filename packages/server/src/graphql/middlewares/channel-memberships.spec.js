const { setContextChannelMemberships } = require('./channel-memberships');
const data = require('../../data');

describe('Channel Memberships Middlewares', () => {

  describe('setContextChannelMemberships() sets agent-channel model into context', () => {
    const channelMembership = {};
    beforeEach(() => {
      jest.spyOn(data.channelMemberships, 'getOne2').mockReturnValue(
        Promise.resolve(channelMembership),
      );
    });

    it('when arguments are passed', () => {
      const root = {};
      const args = { channelId: 'channelId1' };
      const context = { agent: { id: 'agentId1' } };
      const info = {};
      return setContextChannelMemberships(root, args, context, info).then((result) => {
        expect(data.channelMemberships.getOne2).toHaveBeenCalledWith({
          "agentId": "agentId1",
          "channelId": "channelId1"
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
      return setContextChannelMemberships(root, args, context, info).then((result) => {
        expect(data.channelMemberships.getOne2).toHaveBeenCalledWith({
          "agentId": "agentId2",
          "channelId": "channelId1"
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
        channelId: 'channelId1'
      };
      const info = {};
      return setContextChannelMemberships(root, args, context, info).then((result) => {
        expect(data.channelMemberships.getOne2).toHaveBeenCalledWith({
          "agentId": "agentId3",
          "channelId": "channelId1"
        });
        expect(result).toBe(undefined);
        expect(context.channelMembership).toBe(channelMembership);
      });
    });

  });

});

