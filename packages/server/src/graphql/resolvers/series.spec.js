const series = require('./series');
const { SeriesData } = require('../../data');

describe('Series Resolvers', () => {
  describe('one()', () => {
    const root = {};
    const args = { id: 'id1' };
    const context = { agent: { id: 'agentId1' } };
    const info = {};

    it('returns series', () => {
      return series.one(root, args, context, info).then((result) => {
        expect(new SeriesData().getOne).toHaveBeenCalledWith(
          { id: 'id1' },
          {},
          { agentId: 'agentId1', isAdmin: null, raw: true },
        );
        expect(result).toEqual(true);
      });
    });
  });

  describe('all()', () => {
    const root = {};
    const args = {};
    const context = { agent: { id: 'agentId2' } };
    const info = {};

    it('returns series collection', () => {
      return series.all(root, args, context, info).then((result) => {
        expect(new SeriesData().getAll).toHaveBeenCalledWith(
          { channelId: null },
          {},
          { agentId: 'agentId2', isAdmin: null, raw: true },
        );
        expect(result).toEqual(true);
      });
    });
  });

  describe('create()', () => {
    const root = {};
    const args = { input: { a: 'a1' } };
    const context = { agent: { id: 'agentId3' } };
    const info = {};

    it('creates series', () => {
      return series.create(root, args, context, info).then((result) => {
        expect(new SeriesData().createOne).toHaveBeenCalledWith(
          { a: 'a1', creatorId: 'agentId3' },
        );
        expect(result).toBe(true);
      });
    });
  });
});
