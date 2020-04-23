const measurables = require('./measurables');
const { MeasurablesData } = require('../../data');

describe('Measurables Resolvers', () => {
  describe('all()', () => {
    const root = {};
    const args = {
      creatorId: 'creatorId1',
      seriesId: 'seriesId2',
      channelId: 'channelId2',
      states: ['states1'],
      after: 2,
      before: 2,
      last: 'last1',
      first: 'first1',
      isArchived: [false],
    };
    const context = { agent: { id: 'agentId2' } };
    const info = {};

    it('returns measurables', () => {
      return measurables.all(root, args, context, info).then((result) => {
        expect(new MeasurablesData().getConnection).toHaveBeenCalledWith(
          {
            channelId: 'channelId2',
            creatorId: 'creatorId1',
            isArchived: [false],
            seriesId: 'seriesId2',
            states: ['states1'],
            withinJoinedChannels: null,
            measurableIds: null,
            measuredByAgentId: null,
            labelProperty: null,
            labelSubject: null,
          },
          {
            after: 2,
            before: 2,
            first: 'first1',
            last: 'last1',
            _context: {}
          },
          {
            agentId: 'agentId2',
            attributes: true,
            isAdmin: null,
            raw: true,
          },
        );
        expect(result).toEqual(true);
      });
    });
  });

  describe('one()', () => {
    const root = {};
    const args = { id: 'id1' };
    const context = { agent: { id: 'agentId2' } };
    const info = {};

    it('returns a measurable', () => {
      return measurables.one(root, args, context, info).then((result) => {
        expect(new MeasurablesData().getOne).toHaveBeenCalledWith(
          { id: 'id1' },
          {},
          { agentId: 'agentId2', isAdmin: null, raw: true },
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('create()', () => {
    const root = {};
    const args = {};
    const context = { agent: { id: 'agentId1' } };
    const info = {};

    it('creates a measurable', () => {
      return measurables.create(root, args, context, info).then((result) => {
        expect(new MeasurablesData().createOne).toHaveBeenCalledWith(
          { creatorId: 'agentId1' },
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('archive()', () => {
    const root = {};
    const args = { id: 'id1' };
    const context = { agent: {} };
    const info = {};

    it('archives a measurable', () => {
      return measurables.archive(root, args, context, info).then((result) => {
        expect(new MeasurablesData().archive).toHaveBeenCalledWith(
          args.id,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('unarchive()', () => {
    const root = {};
    const args = { id: 'id1' };
    const context = { agent: {} };
    const info = {};

    it('unarchives a measurable', () => {
      return measurables.unarchive(root, args, context, info).then((result) => {
        expect(new MeasurablesData().unArchive).toHaveBeenCalledWith(
          args.id,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('update()', () => {
    const root = {};
    const args = { id: 'id1', input: { b: 'b1' } };
    const context = { agent: {} };
    const info = {};

    it('updates a measurable', () => {
      return measurables.update(root, args, context, info).then((result) => {
        expect(new MeasurablesData().updateOne).toHaveBeenCalledWith(
          { id: 'id1' },
          { b: 'b1' },
        );
        expect(result).toBe(true);
      });
    });
  });
});
