const channels = require('./channels');
const data = require('../data');

describe('Channels Resolvers', () => {

  it('class should be constructor', () => {
    expect(channels.channelAgents).toBeInstanceOf(Function);
    expect(channels.channelCreator).toBeInstanceOf(Function);
  });

  describe('channelAgents()', () => {
    const channel = { id: '1' };
    beforeEach(() => {
      jest.spyOn(data.channelsData, 'getAgentsByChannelId').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns agent-channel model', () => {
      return channels.channelAgents(channel).then((result) => {
        expect(data.channelsData.getAgentsByChannelId)
          .toHaveBeenCalledWith(channel.id);
        expect(result).toBe(true);
      });
    });
  });

  describe('channelCreator()', () => {
    const channel = { id: '1' };
    beforeEach(() => {
      jest.spyOn(data.channelsData, 'getCreatorByChannelId').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns creator of channel', () => {
      return channels.channelCreator(channel).then((result) => {
        expect(data.channelsData.getCreatorByChannelId)
          .toHaveBeenCalledWith(channel.id);
        expect(result).toBe(true);
      });
    });
  });

  describe('all()', () => {
    const root = {};
    const context = { user: { agentId: '1' } };
    const args = { offset: 1, limit: 2 };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channelsData, 'getAll').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns channels with restrictions', () => {
      return channels.all(root, args, context, info).then((result) => {
        expect(data.channelsData.getAll).toHaveBeenCalledWith({
          "limit": 2,
          "offset": 1,
          "agentId": "1"
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('one()', () => {
    const root = {};
    const context = { user: { agentId: 'agentId1' } };
    const args = { id: 'id1' };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channelsData, 'getOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns channel using restrictions', () => {
      return channels.one(root, args, context, info).then((result) => {
        expect(data.channelsData.getOne).toHaveBeenCalledWith("id1", {
          "agentId": "agentId1"
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('update()', () => {
    const root = {};
    const context = {};
    const args = { id: 'id2', input: { a: '1' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channelsData, 'updateOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('updates channel', () => {
      return channels.update(root, args, context, info).then((result) => {
        expect(data.channelsData.updateOne).toHaveBeenCalledWith("id2", {
          "a": "1",
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('create()', () => {
    const root = {};
    const context = { user: { b: '2' } };
    const args = { input: { a: '1' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channelsData, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates channel', () => {
      return channels.create(root, args, context, info).then((result) => {
        expect(data.channelsData.createOne).toHaveBeenCalledWith(
          context.user,
          args.input,
        );
        expect(result).toBe(true);
      });
    });
  });

});
