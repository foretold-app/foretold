const measurables = require('./measurables');
const data = require('../../data');

describe('Measurables Resolvers', () => {
  describe('all()', () => {
    const root = {};
    const args = {
      creatorId: 'creatorId1',
      seriesId: 'seriesId2',
      channelId: 'channelId2',
      states: 'states1',
      after: 2,
      before: 2,
      last: 'last1',
      first: 'first1',
      isArchived: 'isArchived1',
    };
    const context = { agent: { id: 'agentId2' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurables, 'getConnection').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns measurables', () => {
      return measurables.all(root, args, context, info).then((result) => {
        expect(data.measurables.getConnection).toHaveBeenCalledWith(
          {
            channelId: 'channelId2',
            creatorId: 'creatorId1',
            isArchived: 'isArchived1',
            seriesId: 'seriesId2',
            states: 'states1',
            withinJoinedChannels: null,
          },
          {
            after: 2,
            before: 2,
            first: 'first1',
            last: 'last1',
          },
          { agentId: 'agentId2' },
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
    beforeEach(() => {
      jest.spyOn(data.measurables, 'getOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns a measurable', () => {
      return measurables.one(root, args, context, info).then((result) => {
        expect(data.measurables.getOne).toHaveBeenCalledWith(
          { id: 'id1' },
          {},
          { agentId: 'agentId2', isAdmin: undefined },
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
    beforeEach(() => {
      jest.spyOn(data.measurables, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates a measurable', () => {
      return measurables.create(root, args, context, info).then((result) => {
        expect(data.measurables.createOne).toHaveBeenCalledWith(
          { creatorId: 'agentId1' },
          context.user,
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
    beforeEach(() => {
      jest.spyOn(data.measurables, 'archive').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('archives a measurable', () => {
      return measurables.archive(root, args, context, info).then((result) => {
        expect(data.measurables.archive).toHaveBeenCalledWith(
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
    beforeEach(() => {
      jest.spyOn(data.measurables, 'unArchive').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('unarchives a measurable', () => {
      return measurables.unarchive(root, args, context, info).then((result) => {
        expect(data.measurables.unArchive).toHaveBeenCalledWith(
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
    beforeEach(() => {
      jest.spyOn(data.measurables, 'updateOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('updates a measurable', () => {
      return measurables.update(root, args, context, info).then((result) => {
        expect(data.measurables.updateOne).toHaveBeenCalledWith(
          'id1',
          { b: 'b1' },
          context.user,
        );
        expect(result).toBe(true);
      });
    });
  });
});
