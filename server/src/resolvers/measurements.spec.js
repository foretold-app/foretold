const { resolver } = require("graphql-sequelize");

const measurements = require('./measurements');
const data = require('../data');
const models = require('../models');

describe('channels', () => {
  afterEach(() => {
    jest.clearAllMocks();
    jest.restoreAllMocks();
  });

  describe('all', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    it('all', () => {
      return measurements.all(root, args, context, info).then((result) => {
        expect(resolver).toHaveBeenCalledWith(models.Measurement);
        expect(result).toEqual([root, args, context, info]);
      });
    });
  });

  describe('one', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    it('one', () => {
      return measurements.one(root, args, context, info).then((result) => {
        expect(resolver).toHaveBeenCalledWith(models.Measurement);
        expect(result).toEqual([root, args, context, info]);
      });
    });
  });

  describe('create', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    beforeAll(() => {
      jest.spyOn(data.measurementData, 'createMeasurement').mockReturnValue(Promise.resolve(true));
    });
    it('create', () => {
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
