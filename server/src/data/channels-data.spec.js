const models = require('../models');
const { ChannelsData } = require('./channels-data');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('ChannelsData', () => {

  it('class should be constructor', () => {
    expect(ChannelsData).toBeInstanceOf(Function);
  });

  const instance = new ChannelsData();
  const id = 'someId1';
  const user = { agentId: 'agentId1' };
  const input = { name: 'Input Name 1' };

  describe('createOne - branch A', () => {
    beforeEach(() => {
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
    beforeEach(() => {
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
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve({ update }));
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
    beforeEach(() => {
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
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve({ agents }));
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
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve({ creator }));
    });
    it('createOne', () => {
      return instance.getCreatorByChannelId(id).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledTimes(1);
        expect(result).toBe(creator);
      });
    });
  });

  describe('getAll', () => {
    const options = { limit: 1, offset: 2 };
    beforeEach(() => {
      jest.spyOn(instance, 'getRestrictionsSync').mockReturnValue({ query: '1' });
      jest.spyOn(models.Channel, 'findAll').mockReturnValue(Promise.resolve(true));
    });
    it('getAll', () => {
      return instance.getAll(options).then((result) => {
        expect(instance.getRestrictionsSync).toHaveBeenCalledWith(options);
        expect(models.Channel.findAll).toHaveBeenCalledWith({
          "limit": 1,
          "offset": 2,
          "order": [["createdAt", "DESC"]],
          "query": "1"
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getOne', () => {
    const id = 'id1';
    const options = {};
    beforeEach(() => {
      jest.spyOn(instance, 'getRestrictionsSync').mockReturnValue({ query: '1' });
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(true));
    });
    it('getOne', () => {
      return instance.getOne(id, options).then((result) => {
        expect(instance.getRestrictionsSync).toHaveBeenCalledWith(options);
        expect(models.Channel.findOne).toHaveBeenCalledWith({
          "query": "1",
          "where": { "id": "id1" }
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getRestrictionsSync', () => {
    const options = { restrictions: { r1: 'r1', channelIds: 'channelIds1' } };
    beforeEach(() => {
      instance.getRestrictionsSync.mockRestore();
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(true));
    });
    it('getRestrictionsSync', () => {
      const result = instance.getRestrictionsSync(options);
      expect(result).toEqual({
        "where": {
          "orop": [{
            "id": "channelIds1",
            "isPublic": false
          }, { "isPublic": true }]
        },
      });
    });
  });

});
