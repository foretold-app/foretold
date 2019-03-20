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

  describe('deleteOne - branch A', () => {
    beforeAll(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReset().mockReturnValue(Promise.resolve(true));
      jest.spyOn(models.AgentsChannels, 'destroy').mockReset().mockReturnValue(Promise.resolve(true));
    });
    it('deleteOne', () => {
      return instance.deleteOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({ where: input });
        expect(models.AgentsChannels.destroy).toHaveBeenCalledWith({ where: input });
        expect(result).toBe(true);
      });
    });
  });

  describe('deleteOne - branch B', () => {
    beforeAll(() => {
      models.AgentsChannels.findOne.mockReset();
      models.AgentsChannels.destroy.mockReset();
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(Promise.resolve(false));
    });
    it('deleteOne', () => {
      return instance.deleteOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({ where: input });
        expect(models.AgentsChannels.destroy).toHaveBeenCalledTimes(0);
        expect(result).toBe(false);
      });
    });
  });
});
