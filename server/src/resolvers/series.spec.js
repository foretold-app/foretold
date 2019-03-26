const { resolver } = require("graphql-sequelize");

const series = require('./series');
const data = require('../data');
const models = require('../models');

describe('Series Resolvers', () => {

  describe('one()', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    it('returns series', () => {
      return series.one(root, args, context, info).then((result) => {
        expect(resolver).toHaveBeenCalledWith(models.Series);
        expect(result).toEqual([root, args, context, info]);
      });
    });
  });

  describe('all()', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    it('returns series collection', () => {
      return series.all(root, args, context, info).then((result) => {
        expect(resolver).toHaveBeenCalledWith(models.Series);
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
      jest.spyOn(data.seriesData, 'createSeries').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates series', () => {
      return series.create(root, args, context, info).then((result) => {
        expect(data.seriesData.createSeries).toHaveBeenCalledWith(
          root, args, context,
        );
        expect(result).toBe(true);
      });
    });
  });


});
