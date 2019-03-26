const models = require('../models');
const { ChannelsData } = require('./channels-data');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('tests Channels Data layer', () => {

  it('class should be constructor', () => {
    expect(ChannelsData).toBeInstanceOf(Function);
  });

  const instance = new ChannelsData();
  const id = 'someId1';
  const user = { agentId: 'agentId1' };
  const input = { name: 'Input Name 1' };

  describe('tests createOne method', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(true));
      jest.spyOn(models.Channel, 'create').mockReturnValue(Promise.resolve(true));
      jest.spyOn(AgentsChannelsData.prototype, 'createOne').mockReturnValue(Promise.resolve(true));
    });
    it('should find channel', () => {
      return instance.createOne(user, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({ where: { name: input.name } });
        expect(models.Channel.create).toHaveBeenCalledTimes(0);
        expect(AgentsChannelsData.prototype.createOne).toHaveBeenCalledTimes(0);
        expect(result).toBe(true);
      });
    });
  });

  describe('tests createOne method if there is no channel', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(false));
      jest.spyOn(models.Channel, 'create').mockReturnValue(Promise.resolve(true));
      jest.spyOn(AgentsChannelsData.prototype, 'createOne').mockReturnValue(Promise.resolve(true));
    });
    it('should create channel', () => {
      return instance.createOne(user, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({ where: { name: input.name } });
        expect(models.Channel.create).toHaveBeenCalledTimes(1);
        expect(AgentsChannelsData.prototype.createOne).toHaveBeenCalledTimes(1);
        expect(result).toBe(true);
      });
    });
  });

  describe('test updateOne method', () => {
    const update = jest.fn(() => Promise.resolve(true));
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve({ update }));
    });
    it('should fine channel', () => {
      return instance.updateOne(id, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({ where: { id } });
        expect(update).toHaveBeenCalledTimes(1);
      });
    });
  });

  describe('tests updateOne method if there is no channel', () => {
    const update = jest.fn(() => Promise.resolve(true));
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(false));
    });
    it('should not update channel if it is not found', () => {
      return instance.updateOne(id, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({ where: { id } });
        expect(update).toHaveBeenCalledTimes(0);
        expect(result).toBe(false);
      });
    });
  });

  describe('tests getAgentsByChannelId method', () => {
    const agents = [];
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve({ agents }));
    });
    it('should fine channel and return agents', () => {
      return instance.getAgentsByChannelId(id).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledTimes(1);
        expect(result).toBe(agents);
      });
    });
  });

  describe('tests getCreatorByChannelId method', () => {
    const creator = 1;
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve({ creator }));
    });
    it('should find channel and return creator', () => {
      return instance.getCreatorByChannelId(id).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledTimes(1);
        expect(result).toBe(creator);
      });
    });
  });

  describe('tests getAll method', () => {
    const options = { limit: 1, offset: 2 };
    beforeEach(() => {
      jest.spyOn(instance, 'getRestrictionsSync').mockReturnValue({ query: '1' });
      jest.spyOn(models.Channel, 'findAll').mockReturnValue(Promise.resolve(true));
    });
    it('should format restrictions and find all channels', () => {
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

  describe('tests getOne method', () => {
    const id = 'id1';
    const options = {};
    beforeEach(() => {
      jest.spyOn(instance, 'getRestrictionsSync').mockReturnValue({ query: '1' });
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(true));
    });
    it('should find channel', () => {
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

  describe('tests getRestrictionsSync methods', () => {
    const options = { restrictions: { r1: 'r1', channelIds: 'channelIds1' } };
    beforeEach(() => {
      instance.getRestrictionsSync.mockRestore();
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(true));
    });
    it('should return conditions for seek channels', () => {
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
