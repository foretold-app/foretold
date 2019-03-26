const { measurable } = require('./measurables');
const data = require('../data');

describe('measurables', () => {

  describe('measurable ', () => {
    const measurableObj = { channelId: 'channelId1' };
    beforeEach(() => {
      jest.spyOn(data.measurablesData, 'getOne').mockReturnValue(Promise.resolve(measurableObj));
    });

    it('A', () => {
      const root = {};
      const args = { measurableId: 'measurableId1' };
      const context = {};
      const info = {};
      return measurable(root, args, context, info).then((result) => {
        expect(data.measurablesData.getOne).toHaveBeenCalledWith('measurableId1');
        expect(result).toBe(undefined);
        expect(context.measurable).toBe(measurableObj);
        expect(context.channelId).toBe(measurableObj.channelId);
      });
    });

    it('B', () => {
      const root = { measurableId: 'measurableId1' };
      const args = {};
      const context = {};
      const info = {};
      return measurable(root, args, context, info).then((result) => {
        expect(data.measurablesData.getOne).toHaveBeenCalledWith('measurableId1');
        expect(result).toBe(undefined);
        expect(context.measurable).toBe(measurableObj);
        expect(context.channelId).toBe(measurableObj.channelId);
      });
    });

    it('C', () => {
      const root = {};
      const args = {};
      const context = { measurableId: 'measurableId1' };
      const info = {};
      return measurable(root, args, context, info).then((result) => {
        expect(data.measurablesData.getOne).toHaveBeenCalledWith('measurableId1');
        expect(result).toBe(undefined);
        expect(context.measurable).toBe(measurableObj);
        expect(context.channelId).toBe(measurableObj.channelId);
      });
    });

    it('C', () => {
      const root = {};
      const args = { id: 'measurableId1' };
      const context = {};
      const info = {};
      return measurable(root, args, context, info).then((result) => {
        expect(data.measurablesData.getOne).toHaveBeenCalledWith('measurableId1');
        expect(result).toBe(undefined);
        expect(context.measurable).toBe(measurableObj);
        expect(context.channelId).toBe(measurableObj.channelId);
      });
    });

    it('D', () => {
      const root = {};
      const args = {};
      const context = {};
      const info = {};
      return measurable(root, args, context, info).then((result) => {
        expect(result).toBe(undefined);
      });
    });

  });

});

