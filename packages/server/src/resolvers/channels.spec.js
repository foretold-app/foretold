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
      jest.spyOn(data.channels, 'getAgentsByChannelId').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns agent-channel model', () => {
      return channels.channelAgents(channel).then((result) => {
        expect(data.channels.getAgentsByChannelId)
          .toHaveBeenCalledWith(channel.id);
        expect(result).toBe(true);
      });
    });
  });

  describe('channelCreator()', () => {
    const channel = { id: '1' };
    beforeEach(() => {
      jest.spyOn(data.channels, 'getCreatorByChannelId').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns creator of channel', () => {
      return channels.channelCreator(channel).then((result) => {
        expect(data.channels.getCreatorByChannelId)
          .toHaveBeenCalledWith(channel.id);
        expect(result).toBe(true);
      });
    });
  });

  describe('all()', () => {
    const root = {};
    const context = { agent: { id: '1' } };
    const args = { offset: 1, limit: 2, channelMemberId: 'channelMemberId1' };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channels, 'getAll').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns channels with restrictions', () => {
      return channels.all(root, args, context, info).then((result) => {
        expect(data.channels.getAll).toHaveBeenCalledWith({
          "isArchived": undefined,
          "withinJoinedChannels": { "agentId": "channelMemberId1", "as": "id" }
        }, {
          "limit": 2,
          "offset": 1,
        }, {
          "agentId": "1"
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('one()', () => {
    const root = {};
    const context = { agent: { id: 'agentId1' } };
    const args = { id: 'id1' };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channels, 'getOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns channel using restrictions', () => {
      return channels.one(root, args, context, info).then((result) => {
        expect(data.channels.getOne).toHaveBeenCalledWith({ id: "id1" }, {}, {
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
      jest.spyOn(data.channels, 'updateOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('updates channel', () => {
      return channels.update(root, args, context, info).then((result) => {
        expect(data.channels.updateOne).toHaveBeenCalledWith({ id: "id2" }, {
          "a": "1",
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('create()', () => {
    const root = {};
    const context = { agent: { b: '2' } };
    const args = { input: { a: '1' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.channels, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates channel', () => {
      return channels.create(root, args, context, info).then((result) => {
        expect(data.channels.createOne).toHaveBeenCalledWith(
          context.agent,
          args.input,
        );
        expect(result).toBe(true);
      });
    });
  });

});
