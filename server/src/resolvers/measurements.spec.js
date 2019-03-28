const measurements = require('./measurements');
const data = require('../data');

describe('Measurements Resolver', () => {

  describe('all()', () => {
    const root = {};
    const args = {};
    const context = { user: { agentId: 'agentId1' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurementData, 'getAll').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns measurements', () => {
      return measurements.all(root, args, context, info).then((result) => {
        expect(data.measurementData.getAll).toHaveBeenCalledWith(
          { agentId: 'agentId1' },
        );
        expect(result).toEqual(true);
      });
    });
  });

  describe('one()', () => {
    const root = {};
    const args = { id: 'id1' };
    const context = { user: { agentId: 'agentId1' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurementData, 'getOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('return a measurement', () => {
      return measurements.one(root, args, context, info).then((result) => {
        expect(data.measurementData.getOne).toHaveBeenCalledWith(
          'id1',
          { agentId: 'agentId1' },
        );
        expect(result).toEqual(true);
      });
    });
  });

  describe('create()', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurementData, 'createMeasurement').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates a measurement', () => {
      return measurements.create(root, args, context, info).then((result) => {
        expect(data.measurementData.createMeasurement).toHaveBeenCalledWith(
          root,
          args,
          context,
        );
        expect(result).toBe(true);
      });
    });
  });


});
