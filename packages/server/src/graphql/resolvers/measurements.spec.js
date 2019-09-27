const measurements = require('./measurements');
const data = require('../../data');

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
      jest.spyOn(data.measurements, 'getConnection').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('returns measurements', () => {
      return measurements.all(root, args, context, info).then((result) => {
        expect(data.measurements.getConnection).toHaveBeenCalledWith(
          {
            agentId: 'agentId2',
            competitorType: undefined,
            findInDateRange: undefined,
            measurableId: 'measurableId1',
            notTaggedByAgent: undefined,
            withinMeasurables: null,
          },
          {
            after: 3,
            before: 4,
            first: 'first5',
            last: 'last5',
          },
          { agentId: 'agentId1' },
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
      jest.spyOn(data.measurements, 'getOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('return a measurement', () => {
      return measurements.one(root, args, context, info).then((result) => {
        expect(data.measurements.getOne).toHaveBeenCalledWith(
          { id: 'id1' },
          {},
          { agentId: 'agentId2' },
        );
        expect(result).toEqual(true);
      });
    });
  });
});
