const models = require('../models');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('AgentsChannelsData', () => {
  afterEach(() => {
    jest.clearAllMocks();
    jest.restoreAllMocks();
  });

  it('class should be constructor', () => {
    expect(AgentsChannelsData).toBeInstanceOf(Function);
  });

  const instance = new AgentsChannelsData();
  const channelId = 'channelId1';
  const agentId = 'agentId1';
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
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(Promise.resolve(true));
      jest.spyOn(models.AgentsChannels, 'destroy').mockReturnValue(Promise.resolve(true));
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

  describe('validate - branch A', () => {
    it('validate', () => {
      return instance.validate(input).then((result) => {
        expect(models.Channel.findById).toHaveBeenCalledWith(channelId);
        expect(models.Agent.findById).toHaveBeenCalledWith(agentId);
        expect(result).toBe(true);
      });
    });
  });

  describe('validate - branch B', () => {
    beforeAll(() => {
      jest.spyOn(models.Channel, 'findById').mockReturnValue(Promise.resolve(false));
    });
    it('validate ', () => {
      return instance.validate({ agentId }).catch((err) => {
        expect(err).toBeInstanceOf(Error)
      });
    });
  });

  describe('validate - branch C', () => {
    beforeAll(() => {
      jest.spyOn(models.Channel, 'findById').mockReturnValue(Promise.resolve(false));
      jest.spyOn(models.Agent, 'findById').mockReturnValue(Promise.resolve(false));
    });
    it('validate ', () => {
      return instance.validate({ agentId }).catch((err) => {
        expect(models.Channel.findById).toHaveBeenCalledTimes(1);
        expect(models.Agent.findById).toHaveBeenCalledTimes(1);
        expect(err).toBeInstanceOf(Error)
      });
    });
  });
});

