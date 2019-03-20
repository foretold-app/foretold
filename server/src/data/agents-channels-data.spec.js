jest.mock('../models');

const models = require('../models');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('AgentsChannelsData', () => {
  it('class should be constructor', () => {
    expect(AgentsChannelsData).toBeInstanceOf(Function);
  });
  it('createOne', () => {
    const instance = new AgentsChannelsData();
    const channelId = 'id1';
    const agentId = 'id2';
    const input = { channelId, agentId };
    return instance.createOne(channelId, agentId).then((result) => {
      expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({ where: input });
      expect(models.AgentsChannels.create).toHaveBeenCalledTimes(0);
      expect(result).toBe(true);
    });
  });
});
