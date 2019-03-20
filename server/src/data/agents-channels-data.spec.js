jest.mock('../models');

const models = require('../models');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('AgentsChannelsData', () => {
  it('class should be constructor', () => {
    expect(AgentsChannelsData).toBeInstanceOf(Function);
  });

  const instance = new AgentsChannelsData();
  const channelId = 'id1';
  const agentId = 'id2';
  const input = { channelId, agentId };

  describe('createOne - branch A', () => {
    beforeAll(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(Promise.resolve(true));
    });
    it('createOne', () => {
      return instance.createOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({ where: input });
        expect(models.AgentsChannels.create).toHaveBeenCalledTimes(0);
        expect(result).toBe(true);
      });
    });
  });

  describe('createOne - branch B', () => {
    beforeAll(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(Promise.resolve(false));
    });
    it('createOne', () => {
      return instance.createOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({ where: input });
        expect(models.AgentsChannels.create).toHaveBeenCalledWith(input);
        expect(result).toBe(true);
      });
    });
  });
});
