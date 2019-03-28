const models = require('../models');
const { ChannelsData } = require('./channels-data');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('Channels Data Layer', () => {

  it('class should be constructor', () => {
    expect(ChannelsData).toBeInstanceOf(Function);
  });

  const instance = new ChannelsData();
  const id = 'someId1';
  const user = { agentId: 'agentId1' };
  const input = { name: 'Input Name 1' };

  describe('createOne()', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(models.Channel, 'create').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(AgentsChannelsData.prototype, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('throw an error that channel is exists', () => {
      instance.createOne(user, input).catch((err) => {
        expect(err).toBeInstanceOf(Error);
      });
    });
  });

  describe('createOne() if there is no channel', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(
        Promise.resolve(false),
      );
      jest.spyOn(models.Channel, 'create').mockReturnValue(
        Promise.resolve({ id: 'id1' }),
      );
      jest.spyOn(AgentsChannelsData.prototype, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates channel', () => {
      return instance.createOne(user, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({
          where: { name: input.name },
        });
        expect(models.Channel.create).toHaveBeenCalledTimes(1);
        expect(AgentsChannelsData.prototype.createOne).toHaveBeenCalledTimes(1);
        expect(AgentsChannelsData.prototype.createOne).toHaveBeenCalledWith(
          "id1",
          "agentId1",
          ["admin", "viewer"]
        );
        expect(result).toEqual({ id: 'id1' });
      });
    });
  });

  describe('updateOne()', () => {
    const update = jest.fn(() => Promise.resolve(true));
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(
        Promise.resolve({ update }),
      );
    });
    it('finds channel', () => {
      return instance.updateOne(id, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({ where: { id } });
        expect(update).toHaveBeenCalledTimes(1);
      });
    });
  });

  describe('updateOne() if there is no channel', () => {
    const update = jest.fn(() => Promise.resolve(true));
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('does not update channel when it is not found', () => {
      return instance.updateOne(id, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({
          where: { id },
        });
        expect(update).toHaveBeenCalledTimes(0);
        expect(result).toBe(false);
      });
    });
  });

  describe('getAgentsByChannelId()', () => {
    const agents = [];
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(
        Promise.resolve({ agents }),
      );
    });
    it('finds channel and returns agents', () => {
      return instance.getAgentsByChannelId(id).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledTimes(1);
        expect(result).toBe(agents);
      });
    });
  });

  describe('getCreatorByChannelId()', () => {
    const creator = 1;
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(
        Promise.resolve({ creator }),
      );
    });
    it('finds channel and returns creator', () => {
      return instance.getCreatorByChannelId(id).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledTimes(1);
        expect(result).toBe(creator);
      });
    });
  });

  describe('getAll()', () => {
    const options = { limit: 1, offset: 2 };
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findAll').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(instance, 'channelIdsLiteral').mockReturnValue(
        'channelIdsLiteral',
      );
    });
    it('formats restrictions and finds all channels', () => {
      return instance.getAll(options).then((result) => {
        expect(models.Channel.findAll).toHaveBeenCalledWith({
          "limit": 1,
          "offset": 2,
          "order": [["createdAt", "DESC"]],
          "where": { "id": { "$in": "channelIdsLiteral" } }
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getOne()', () => {
    const id = 'id1';
    const options = { agentId: undefined };
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(instance, 'channelIdsLiteral').mockReturnValue(
        'channelIdsLiteral',
      );
    });
    it('finds channel', () => {
      return instance.getOne(id, options).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({
          "where": {
            "$and": [
              { "id": "id1" },
              { "id": { "$in": "channelIdsLiteral" } }
            ]
          }
        });
        expect(result).toBe(true);
      });
    });
  });

});
