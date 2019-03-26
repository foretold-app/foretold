const { resolver } = require("graphql-sequelize");

const measurables = require('./measurables');
const data = require('../data');
const models = require('../models');

describe('measurables', () => {

  describe('all', () => {
    const root = {};
    const args = { channelId: 'channelId1', agentId: 'agentId2' };
    const context = {};
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurablesData, 'getAll').mockReturnValue(Promise.resolve(true));
    });
    it('all', () => {
      return measurables.all(root, args, context, info).then((result) => {
        expect(data.measurablesData.getAll).toHaveBeenCalledWith(args);
        expect(result).toBe(true);
      });
    });
  });

  describe('one', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    it('one', () => {
      return measurables.one(root, args, context, info).then((result) => {
        expect(resolver).toHaveBeenCalledWith(models.Measurable);
        expect(result).toEqual([root, args, context, info]);
      });
    });
  });

  describe('create', () => {
    const root = {};
    const args = {};
    const context = { user: { agentId: 'agentId1' } };
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurablesData, 'createMeasurable').mockReturnValue(Promise.resolve(true));
    });
    it('create', () => {
      return measurables.create(root, args, context, info).then((result) => {
        expect(data.measurablesData.createMeasurable).toHaveBeenCalledWith(
          { "creatorId": "agentId1" },
          context.user,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('archive', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurablesData, 'archiveMeasurable').mockReturnValue(Promise.resolve(true));
    });
    it('archive', () => {
      return measurables.archive(root, args, context, info).then((result) => {
        expect(data.measurablesData.archiveMeasurable).toHaveBeenCalledWith(
          root,
          args,
          context,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('unarchive', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurablesData, 'unArchiveMeasurable').mockReturnValue(Promise.resolve(true));
    });
    it('unarchive', () => {
      return measurables.unarchive(root, args, context, info).then((result) => {
        expect(data.measurablesData.unArchiveMeasurable).toHaveBeenCalledWith(
          root,
          args,
          context,
        );
        expect(result).toBe(true);
      });
    });
  });

  describe('edit', () => {
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    beforeEach(() => {
      jest.spyOn(data.measurablesData, 'editMeasurable').mockReturnValue(Promise.resolve(true));
    });
    it('edit', () => {
      return measurables.edit(root, args, context, info).then((result) => {
        expect(data.measurablesData.editMeasurable).toHaveBeenCalledWith(
          root,
          args,
          context,
        );
        expect(result).toBe(true);
      });
    });
  });

});
