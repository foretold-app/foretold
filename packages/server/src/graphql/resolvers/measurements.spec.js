const measurements = require('./measurements');
const { MeasurementsData } = require('../../data');

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

    it('returns measurements', () => {
      return measurements.all(root, args, context, info).then((result) => {
        expect(new MeasurementsData().getConnection).toHaveBeenCalledWith(
          {
            agentId: 'agentId2',
            competitorType: null,
            findInDateRange: null,
            isVerified: null,
            measurableId: 'measurableId1',
            notTaggedByAgent: null,
            withinMeasurables: null,
            isCancelled: null,
          },
          {
            after: 3,
            before: 4,
            first: 'first5',
            last: 'last5',
            _context: {},
          },
          { agentId: 'agentId1', isAdmin: null, raw: true },
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

    it('returns a measurement', () => {
      return measurements.one(root, args, context, info).then((result) => {
        expect(new MeasurementsData().getOne).toHaveBeenCalledWith(
          { id: 'id1' },
          {},
          { agentId: 'agentId2', isAdmin: null, raw: true },
        );
        expect(result).toEqual(true);
      });
    });
  });
});
