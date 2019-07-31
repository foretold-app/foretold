const models = require('../models');
const { ChannelsData } = require('./channels-data');
const { ChannelMembershipsData } = require('./channel-memberships-data');

describe('Channels Data Layer', () => {

  it('class should be constructor', () => {
    expect(ChannelsData).toBeInstanceOf(Function);
  });

  const instance = new ChannelsData();
  const id = 'someId1';
  const agent = { id: 'agentId1' };
  const input = { name: 'Input Name 1' };

  describe('createOne()', () => {
    beforeEach(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(models.Channel, 'create').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(ChannelMembershipsData.prototype, 'createOne2').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('throw an error that channel is exists', () => {
      instance.createOne(agent, input).catch((err) => {
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
      jest.spyOn(ChannelMembershipsData.prototype, 'createOne2').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates channel', () => {
      return instance.createOne(agent, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({
          where: { name: input.name },
        });
        expect(models.Channel.create).toHaveBeenCalledTimes(1);
        expect(ChannelMembershipsData.prototype.createOne2).toHaveBeenCalledWith(
          "id1",
          "agentId1",
          "ADMIN"
        );
        expect(result).toEqual({ id: 'id1' });
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

});
