const { resolver } = require("graphql-sequelize");

const measurements = require('./measurements');
const data = require('../data');
const models = require('../models');

describe('Measurements Resolver', () => {

  describe('all()', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    it('returns measurements', () => {
      return measurements.all(root, args, context, info).then((result) => {
        expect(resolver).toHaveBeenCalledWith(models.Measurement);
        expect(result).toEqual([root, args, context, info]);
      });
    });
  });

  describe('one()', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    it('return a measurement', () => {
      return measurements.one(root, args, context, info).then((result) => {
        expect(resolver).toHaveBeenCalledWith(models.Measurement);
        expect(result).toEqual([root, args, context, info]);
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
