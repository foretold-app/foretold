const { ChannelsData } = require('../../data');
const channels = require('./channels');

describe('Channels Resolvers', () => {
  describe('all()', () => {
    const root = {};
    const context = { agent: { id: '1' } };
    const args = { offset: 1, limit: 2, channelMemberId: 'channelMemberId1' };
    const info = {};

    it('returns channels with restrictions', () => {
      return channels.all(root, args, context, info).then((result) => {
        expect(new ChannelsData().getConnection).toHaveBeenCalledWith({
          isArchived: null,
          withinJoinedChannels: { agentId: 'channelMemberId1', as: 'id' },
        }, {
          _context: {
            agentId: '1',
          },
          limit: 2,
          offset: 1,
        }, {
          agentId: '1',
          attributes: {
            bookmarksCount: true,
            isBookmarked: { agentId: '1' },
          },
          raw: true,
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

    it('returns channel using restrictions', () => {
      return channels.one(root, args, context, info).then((result) => {
        expect(new ChannelsData().getOne).toHaveBeenCalledWith({ id: 'id1' }, {}, {
          agentId: 'agentId1',
          attributes: {
            bookmarksCount: true,
            isBookmarked: { agentId: 'agentId1' },
          },
          raw: true,
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

    it('updates channel', () => {
      return channels.update(root, args, context, info).then((result) => {
        expect(new ChannelsData().updateOne).toHaveBeenCalledWith({ id: 'id2' }, {
          a: '1',
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('create()', () => {
    const root = {};
    const context = { agent: { b: '2', id: 'id2' } };
    const args = { input: { a: '1' } };
    const info = {};

    it('creates channel', () => {
      return channels.create(root, args, context, info).then((result) => {
        expect(new ChannelsData().createOne).toHaveBeenCalledWith(
          { a: '1', creatorId: 'id2' },
        );
        expect(result).toBe(true);
      });
    });
  });
});
