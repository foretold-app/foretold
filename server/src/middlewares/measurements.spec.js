const { measurement } = require('./measurements');
const data = require('../data');

describe('measurements', () => {

  describe('measurement ', () => {
    const measurementObj = { measurableId: 'measurableId1' };
    beforeEach(() => {
      jest.spyOn(data.measurementData, 'getOne').mockReturnValue(
        Promise.resolve(measurementObj),
      );
    });

    it('A', () => {
      const root = {};
      const args = { id: 'measurementId1' };
      const context = {};
      const info = {};
      return measurement(root, args, context, info).then((result) => {
        expect(data.measurementData.getOne).toHaveBeenCalledWith('measurementId1');
        expect(result).toBe(undefined);
        expect(context.measurement).toBe(measurementObj);
        expect(context.measurableId).toBe(measurementObj.measurableId);
      });
    });

    it('B', () => {
      const root = { measurementId: 'measurementId1' };
      const args = {};
      const context = {};
      const info = {};
      return measurement(root, args, context, info).then((result) => {
        expect(data.measurementData.getOne).toHaveBeenCalledWith('measurementId1');
        expect(result).toBe(undefined);
        expect(context.measurement).toBe(measurementObj);
        expect(context.measurableId).toBe(measurementObj.measurableId);
      });
    });

    it('C', () => {
      const root = {};
      const args = {};
      const context = { measurementId: 'measurementId1' };
      const info = {};
      return measurement(root, args, context, info).then((result) => {
        expect(data.measurementData.getOne).toHaveBeenCalledWith('measurementId1');
        expect(result).toBe(undefined);
        expect(context.measurement).toBe(measurementObj);
        expect(context.measurableId).toBe(measurementObj.measurableId);
      });
    });

    it('D', () => {
      const root = {};
      const args = {};
      const context = {};
      const info = {};
      return measurement(root, args, context, info).then((result) => {
        expect(result).toBe(undefined);
      });
    });

  });

});

