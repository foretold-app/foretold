const measurements = require('./measurements');
const data = require('../data');

describe('Measurements Resolver', () => {

  describe('all()', () => {
    const root = {};
    const args = {
      measurableId: 'measurableId1',
      agentId: 'agentId2',
      after: 3,
      before: 4,
      last: 'last5',
      first: 'first5',
    };
    const context = { agent: { id: 'agentId1' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurements, 'getAll').mockReturnValue(
        Promise.resolve({
          data: true,
          total: 1
        }),
      );
    });
    it('returns measurements', () => {
      return measurements.all(root, args, context, info).then((result) => {
        expect(data.measurements.getAll).toHaveBeenCalledWith(
          { "agentId": "agentId2", "measurableId": "measurableId1" },
          {
            "after": 3,
            "before": 4,
            "first": "first5",
            "last": "last5"
          },
          { "agentId": "agentId1" },
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
      jest.spyOn(data.measurements, 'getOne2').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('return a measurement', () => {
      return measurements.one(root, args, context, info).then((result) => {
        expect(data.measurements.getOne2).toHaveBeenCalledWith(
          'id1',
          { agentId: 'agentId2' },
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
    beforeEach(() => {
      jest.spyOn(data.measurements, 'createOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates a measurement', () => {
      return measurements.create(root, args, context, info).then((result) => {
        expect(data.measurements.createOne).toHaveBeenCalledWith(
          { "a": "a1", "agentId": "agentId3" },
          context.user,
        );
        expect(result).toBe(true);
      });
    });
  });

});
