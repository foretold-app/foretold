const models = require('../models');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('data layer of agent-channels', () => {

  it('class should be a constructor', () => {
    expect(AgentsChannelsData).toBeInstanceOf(Function);
  });

  const instance = new AgentsChannelsData();
  const channelId = 'channelId1';
  const agentId = 'agentId1';
  const input = { channelId, agentId };

  describe('createOne() when there is user', () => {
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds user without creating new one', () => {
      return instance.createOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({
          where: input,
        });
        expect(models.AgentsChannels.create).toHaveBeenCalledTimes(0);
        expect(result).toBe(true);
      });
    });
  });

  describe('createOne() when there is no user', () => {
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('creates new user', () => {
      return instance.createOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({
          where: input,
        });
        expect(models.AgentsChannels.create).toHaveBeenCalledWith(input);
        expect(result).toBe(true);
      });
    });
  });

  describe('deleteOne() when there is user', () => {
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(models.AgentsChannels, 'destroy').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds and destroys user', () => {
      return instance.deleteOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({
          where: input,
        });
        expect(models.AgentsChannels.destroy).toHaveBeenCalledWith({
          where: input,
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('deleteOne() when there is no user', () => {
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('does not destroy user when there is no user', () => {
      return instance.deleteOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({
          where: input,
        });
        expect(models.AgentsChannels.destroy).toHaveBeenCalledTimes(0);
        expect(result).toBe(false);
      });
    });
  });

  describe('validate() if all input is present', () => {
    it('finds channel and agent', () => {
      return instance.validate(input).then((result) => {
        expect(models.Channel.findById).toHaveBeenCalledWith(channelId);
        expect(models.Agent.findById).toHaveBeenCalledWith(agentId);
        expect(result).toBe(true);
      });
    });
  });

  describe('validate() if there is channel', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findById').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('throws an error ', () => {
      return instance.validate(input).catch((err) => {
        expect(err).toBeInstanceOf(Error)
      });
    });
  });

  describe('validate() if there are channel and agent', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findById').mockReturnValue(
        Promise.resolve(false),
      );
      jest.spyOn(models.Agent, 'findById').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('does not find agent', () => {
      return instance.validate({ agentId }).catch((err) => {
        expect(models.Channel.findById).toHaveBeenCalledTimes(1);
        expect(models.Agent.findById).toHaveBeenCalledTimes(0);
        expect(err).toBeInstanceOf(Error)
      });
    });
  });

  describe('getOne()', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds agent-channel note ', () => {
      return instance.getOne(options).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({
          where: options,
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getAll()', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findAll').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('find agents-channels notes', () => {
      return instance.getAll(options).then((result) => {
        expect(models.AgentsChannels.findAll).toHaveBeenCalledWith({
          where: options,
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getAllChannelIds()', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(instance, 'getAll').mockReturnValue(Promise.resolve([
        { channelId: 'channelId1' }
      ]));
    });
    it('calls getAll()', () => {
      return instance.getAllChannelIds(options).then((result) => {
        expect(instance.getAll).toHaveBeenCalledWith(options);
        expect(result).toEqual(['channelId1']);
      });
    });
  });

});

