const { measurement } = require('./measurements');
const data = require('../data');

describe('Measurements Middleware', () => {

  describe('measurement() sets measurement and measurableId into context', () => {
    const measurementObj = { measurableId: 'measurableId1' };
    beforeEach(() => {
      jest.spyOn(data.measurementData, 'getOne').mockReturnValue(
        Promise.resolve(measurementObj),
      );
    });

    it('when arguments are passed', () => {
      const root = {};
      const args = { id: 'measurementId1' };
      const context = {};
      const info = {};
      return measurement(root, args, context, info).then((result) => {
        expect(data.measurementData.getOne)
          .toHaveBeenCalledWith('measurementId1');
        expect(result).toBe(undefined);
        expect(context.measurement).toBe(measurementObj);
        expect(context.measurableId).toBe(measurementObj.measurableId);
      });
    });

    it('when root is passed', () => {
      const root = { measurementId: 'measurementId1' };
      const args = {};
      const context = {};
      const info = {};
      return measurement(root, args, context, info).then((result) => {
        expect(data.measurementData.getOne)
          .toHaveBeenCalledWith('measurementId1');
        expect(result).toBe(undefined);
        expect(context.measurement).toBe(measurementObj);
        expect(context.measurableId).toBe(measurementObj.measurableId);
      });
    });

    it('when context is passed', () => {
      const root = {};
      const args = {};
      const context = { measurementId: 'measurementId1' };
      const info = {};
      return measurement(root, args, context, info).then((result) => {
        expect(data.measurementData.getOne)
          .toHaveBeenCalledWith('measurementId1');
        expect(result).toBe(undefined);
        expect(context.measurement).toBe(measurementObj);
        expect(context.measurableId).toBe(measurementObj.measurableId);
      });
    });

    it('when nothing is passed', () => {
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

