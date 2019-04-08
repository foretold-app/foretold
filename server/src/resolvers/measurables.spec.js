const measurables = require('./measurables');
const data = require('../data');

describe('Measurables Resolvers', () => {

  describe('all()', () => {
    const root = {};
    const args = {};
    const context = { user: { agentId: 'agentId2' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurables, 'getAll').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns measurables', () => {
      return measurables.all(root, args, context, info).then((result) => {
        expect(data.measurables.getAll).toHaveBeenCalledWith(
          { "agentId": "agentId2" },
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('one()', () => {
    const root = {};
    const args = { id: 'id1' };
    const context = { user: { agentId: 'agentId2' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurables, 'getOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns a measurable', () => {
      return measurables.one(root, args, context, info).then((result) => {
        expect(data.measurables.getOne).toHaveBeenCalledWith(
          'id1',
          { "agentId": "agentId2" },
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('create()', () => {
    const root = {};
    const args = {};
    const context = { user: { agentId: 'agentId1' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurables, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates a measurable', () => {
      return measurables.create(root, args, context, info).then((result) => {
        expect(data.measurables.createOne).toHaveBeenCalledWith(
          { "creatorId": "agentId1" },
          context.user,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('archive()', () => {
    const root = {};
    const args = { id: 'id1' };
    const context = { user: {} };
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
    const context = { user: {} };
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
    const context = { user: {} };
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
