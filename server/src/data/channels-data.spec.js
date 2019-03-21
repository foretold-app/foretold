jest.mock('../models');

const models = require('../models');
const { ChannelsData } = require('./channels-data');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('ChannelsData', () => {
  afterEach(() => {
    jest.clearAllMocks();
    jest.restoreAllMocks();
  });

  it('class should be constructor', () => {
    expect(ChannelsData).toBeInstanceOf(Function);
  });

  const instance = new ChannelsData();
  const id = 'someId1';
  const user = { agentId: 'agentId1' };
  const input = { name: 'Input Name 1' };

  describe('createOne - branch A', () => {
    beforeAll(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(true));
      jest.spyOn(models.Channel, 'create').mockReturnValue(Promise.resolve(true));
      jest.spyOn(AgentsChannelsData.prototype, 'createOne').mockReturnValue(Promise.resolve(true));
    });
    it('createOne', () => {
      return instance.createOne(user, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({ where: { name: input.name } });
        expect(models.Channel.create).toHaveBeenCalledTimes(0);
        expect(AgentsChannelsData.prototype.createOne).toHaveBeenCalledTimes(0);
        expect(result).toBe(true);
      });
    });
  });

  describe('createOne - branch B', () => {
    beforeAll(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(false));
      jest.spyOn(models.Channel, 'create').mockReturnValue(Promise.resolve(true));
      jest.spyOn(AgentsChannelsData.prototype, 'createOne').mockReturnValue(Promise.resolve(true));
    });
    it('createOne', () => {
      return instance.createOne(user, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({ where: { name: input.name } });
        expect(models.Channel.create).toHaveBeenCalledTimes(1);
        expect(AgentsChannelsData.prototype.createOne).toHaveBeenCalledTimes(1);
        expect(result).toBe(true);
      });
    });
  });

  describe('updateOne - branch A', () => {
    const update = jest.fn(() => Promise.resolve(true));
    beforeAll(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve({update}));
    });
    it('createOne', () => {
      return instance.updateOne(id, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({ where: { id } });
        expect(update).toHaveBeenCalledTimes(1);
      });
    });
  });

  describe('updateOne - branch B', () => {
    const update = jest.fn(() => Promise.resolve(true));
    beforeAll(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(false));
    });
    it('createOne', () => {
      return instance.updateOne(id, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({ where: { id } });
        expect(update).toHaveBeenCalledTimes(0);
        expect(result).toBe(false);
      });
    });
  });

  describe('getAgentsByChannelId - branch A', () => {
    const agents = [];
    beforeAll(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve({agents}));
    });
    it('createOne', () => {
      return instance.getAgentsByChannelId(id).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledTimes(1);
        expect(result).toBe(agents);
      });
    });
  });

  describe('getCreatorByChannelId - branch A', () => {
    const creator = 1;
    beforeAll(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve({creator}));
    });
    it('createOne', () => {
      return instance.getCreatorByChannelId(id).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledTimes(1);
        expect(result).toBe(creator);
      });
    });
  });

});
