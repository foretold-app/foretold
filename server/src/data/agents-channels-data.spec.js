const models = require('../models');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('tests data layer of agent-channels', () => {

  it('class should be a constructor', () => {
    expect(AgentsChannelsData).toBeInstanceOf(Function);
  });

  const instance = new AgentsChannelsData();
  const channelId = 'channelId1';
  const agentId = 'agentId1';
  const input = { channelId, agentId };

  describe('tests createOne method if there is user', () => {
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('should find user without creating new one', () => {
      return instance.createOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({
          where: input,
        });
        expect(models.AgentsChannels.create).toHaveBeenCalledTimes(0);
        expect(result).toBe(true);
      });
    });
  });

  describe('tests createOne if there is no user', () => {
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('should create new user', () => {
      return instance.createOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({
          where: input,
        });
        expect(models.AgentsChannels.create).toHaveBeenCalledWith(input);
        expect(result).toBe(true);
      });
    });
  });

  describe('tests deleteOne method if there is user', () => {
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(models.AgentsChannels, 'destroy').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('should find and destroy user', () => {
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

  describe('tests deleteOne method if there is no user', () => {
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('should not destroy user if there is no use', () => {
      return instance.deleteOne(channelId, agentId).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({
          where: input,
        });
        expect(models.AgentsChannels.destroy).toHaveBeenCalledTimes(0);
        expect(result).toBe(false);
      });
    });
  });

  describe('tests validate method if all input is present', () => {
    it('should find channel and agent', () => {
      return instance.validate(input).then((result) => {
        expect(models.Channel.findById).toHaveBeenCalledWith(channelId);
        expect(models.Agent.findById).toHaveBeenCalledWith(agentId);
        expect(result).toBe(true);
      });
    });
  });

  describe('tests validate method if there is channel', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findById').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('should throw an error ', () => {
      return instance.validate(input).catch((err) => {
        expect(err).toBeInstanceOf(Error)
      });
    });
  });

  describe('tests validate method if there are channel and agent', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findById').mockReturnValue(
        Promise.resolve(false),
      );
      jest.spyOn(models.Agent, 'findById').mockReturnValue(
        Promise.resolve(false),
      );
    });
    it('should not find agent ', () => {
      return instance.validate({ agentId }).catch((err) => {
        expect(models.Channel.findById).toHaveBeenCalledTimes(1);
        expect(models.Agent.findById).toHaveBeenCalledTimes(0);
        expect(err).toBeInstanceOf(Error)
      });
    });
  });

  describe('tests getOne method', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('should fine agent-channel note ', () => {
      return instance.getOne(options).then((result) => {
        expect(models.AgentsChannels.findOne).toHaveBeenCalledWith({
          where: options,
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('tests getAll method', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(models.AgentsChannels, 'findAll').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('should find agents-channels notes ', () => {
      return instance.getAll(options).then((result) => {
        expect(models.AgentsChannels.findAll).toHaveBeenCalledWith({
          where: options,
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('tests getAllChannelIds method', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(instance, 'getAll').mockReturnValue(Promise.resolve([
        { channelId: 'channelId1' }
      ]));
    });
    it('should call getAll method', () => {
      return instance.getAllChannelIds(options).then((result) => {
        expect(instance.getAll).toHaveBeenCalledWith(options);
        expect(result).toEqual(['channelId1']);
      });
    });
  });

});

