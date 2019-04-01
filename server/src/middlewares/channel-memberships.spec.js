const { channelMemberships } = require('./channel-memberships');
const data = require('../data');

describe('ChannelMemberships Middlewares', () => {

  describe('channelMemberships() sets agent-channel model into context', () => {
    const channelMembership = {};
    beforeEach(() => {
      jest.spyOn(data.channelMembershipsData, 'getOne').mockReturnValue(
        Promise.resolve(channelMembership),
      );
    });

    it('when arguments are passed', () => {
      const root = {};
      const args = { channelId: 'channelId1' };
      const context = { user: { agentId: 'agentId1' } };
      const info = {};
      return channelMemberships(root, args, context, info).then((result) => {
        expect(data.channelMembershipsData.getOne).toHaveBeenCalledWith({
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
      const context = { user: { agentId: 'agentId1' } };
      const info = {};
      return channelMemberships(root, args, context, info).then((result) => {
        expect(data.channelMembershipsData.getOne).toHaveBeenCalledWith({
          "agentId": "agentId1",
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
        user: { agentId: 'agentId1' },
        channelId: 'channelId1'
      };
      const info = {};
      return channelMemberships(root, args, context, info).then((result) => {
        expect(data.channelMembershipsData.getOne).toHaveBeenCalledWith({
          "agentId": "agentId1",
          "channelId": "channelId1"
        });
        expect(result).toBe(undefined);
        expect(context.channelMembership).toBe(channelMembership);
      });
    });

  });

});

