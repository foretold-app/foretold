const { agentsChannels } = require('./agents-channels');
const data = require('../data');

describe('AgentsChannels', () => {

  describe('agentsChannels ', () => {
    const agentChannel = {};
    beforeEach(() => {
      jest.spyOn(data.agentsChannelsData, 'getOne').mockReturnValue(Promise.resolve(agentChannel));
    });

    it('A', () => {
      const root = {};
      const args = { channelId: 'channelId1' };
      const context = { user: { agentId: 'agentId1' } };
      const info = {};
      return agentsChannels(root, args, context, info).then((result) => {
        expect(data.agentsChannelsData.getOne).toHaveBeenCalledWith({
          "agentId": "agentId1",
          "channelId": "channelId1"
        });
        expect(result).toBe(undefined);
        expect(context.agentChannel).toBe(agentChannel);
      });
    });

    it('B', () => {
      const root = { channelId: 'channelId1' };
      const args = {};
      const context = { user: { agentId: 'agentId1' } };
      const info = {};
      return agentsChannels(root, args, context, info).then((result) => {
        expect(data.agentsChannelsData.getOne).toHaveBeenCalledWith({
          "agentId": "agentId1",
          "channelId": "channelId1"
        });
        expect(result).toBe(undefined);
        expect(context.agentChannel).toBe(agentChannel);
      });
    });

    it('C', () => {
      const root = {};
      const args = {};
      const context = {
        user: { agentId: 'agentId1' },
        channelId: 'channelId1'
      };
      const info = {};
      return agentsChannels(root, args, context, info).then((result) => {
        expect(data.agentsChannelsData.getOne).toHaveBeenCalledWith({
          "agentId": "agentId1",
          "channelId": "channelId1"
        });
        expect(result).toBe(undefined);
        expect(context.agentChannel).toBe(agentChannel);
      });
    });

  });

});

