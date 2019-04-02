const measurables = require('./measurables');
const data = require('../data');

describe('Measurables Resolvers', () => {

  describe('all()', () => {
    const root = {};
    const args = {};
    const context = { user: { agentId: 'agentId2' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurablesData, 'getAll').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns measurables', () => {
      return measurables.all(root, args, context, info).then((result) => {
        expect(data.measurablesData.getAll).toHaveBeenCalledWith(
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
      jest.spyOn(data.measurablesData, 'getOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns a measurable', () => {
      return measurables.one(root, args, context, info).then((result) => {
        expect(data.measurablesData.getOne).toHaveBeenCalledWith(
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
      jest.spyOn(data.measurablesData, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates a measurable', () => {
      return measurables.create(root, args, context, info).then((result) => {
        expect(data.measurablesData.createOne).toHaveBeenCalledWith(
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
      jest.spyOn(data.measurablesData, 'archive').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('archives a measurable', () => {
      return measurables.archive(root, args, context, info).then((result) => {
        expect(data.measurablesData.archive).toHaveBeenCalledWith(
          args.id,
          context.user,
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
      jest.spyOn(data.measurablesData, 'unArchive').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('unarchives a measurable', () => {
      return measurables.unarchive(root, args, context, info).then((result) => {
        expect(data.measurablesData.unArchive).toHaveBeenCalledWith(
          args.id,
          context.user,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('update()', () => {
    const root = {};
    const args = { id: 'id1', b: 'b1' };
    const context = { user: {} };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurablesData, 'updateOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('updates a measurable', () => {
      return measurables.update(root, args, context, info).then((result) => {
        expect(data.measurablesData.updateOne).toHaveBeenCalledWith(
          'id1',
          { b: 'b1' },
          context.user,
        );
        expect(result).toBe(true);
      });
    });
  });

});
