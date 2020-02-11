const { setContextMeasurable } = require('./measurables');
const { MeasurablesData } = require('../../data');

describe('Measurables Middleware', () => {
  describe('setContextMeasurable() sets measurable and channelId into context ', () => {
    const measurableObj = true;

    it('when arguments are passed', () => {
      const root = {};
      const args = { measurableId: 'measurableId1' };
      const context = {};
      const info = {};
      return setContextMeasurable(root, args, context, info).then((result) => {
        expect(new MeasurablesData().getOne)
          .toHaveBeenCalledWith({ id: 'measurableId1' }, {}, { raw: true });
        expect(result).toBe(undefined);
        expect(context.measurable).toBe(measurableObj);
      });
    });

    it('when root is passed', () => {
      const root = { measurableId: 'measurableId1' };
      const args = {};
      const context = {};
      const info = {};
      return setContextMeasurable(root, args, context, info).then((result) => {
        expect(new MeasurablesData().getOne)
          .toHaveBeenCalledWith({ id: 'measurableId1' }, {}, { raw: true });
        expect(result).toBe(undefined);
        expect(context.measurable).toBe(measurableObj);
      });
    });

    it('when context is passed', () => {
      const root = {};
      const args = {};
      const context = { measurableId: 'measurableId1' };
      const info = {};
      return setContextMeasurable(root, args, context, info).then((result) => {
        expect(new MeasurablesData().getOne)
          .toHaveBeenCalledWith({ id: 'measurableId1' }, {}, { raw: true });
        expect(result).toBe(undefined);
        expect(context.measurable).toBe(measurableObj);
      });
    });

    it('when arguments are passed', () => {
      const root = {};
      const args = { id: 'measurableId1' };
      const context = {};
      const info = {};
      return setContextMeasurable(root, args, context, info).then((result) => {
        expect(new MeasurablesData().getOne)
          .toHaveBeenCalledWith({ id: 'measurableId1' }, {}, { raw: true });
        expect(result).toBe(undefined);
        expect(context.measurable).toBe(measurableObj);
      });
    });

    it('when nothing is passed', () => {
      const root = {};
      const args = {};
      const context = {};
      const info = {};
      return setContextMeasurable(root, args, context, info).then((result) => {
        expect(result).toBe(undefined);
      });
    });
  });
});
