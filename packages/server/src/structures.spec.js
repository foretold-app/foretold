const index = require('./structures');
const { withinMeasurables } = require('./structures');

describe('Structure Factories', () => {
  it('should be', () => {
    expect(!!index).toBe(true);
  });
  it('withinMeasurables()', () => {
    const states = ['a', 'b', 'c'];
    const channelId = 'channelId1';
    expect(withinMeasurables(states, channelId)).toEqual(
      {
        "as": "measurableId",
        "channelId": "channelId1",
        "states": [
          "a",
          "b",
          "c",
        ],
      }
    );
    expect(withinMeasurables(states)).toEqual(
      {
        "as": "measurableId",
        "states": [
          "a",
          "b",
          "c",
        ],
      }
    );
    expect(withinMeasurables(null, channelId)).toEqual(
      {
        "as": "measurableId",
        "channelId": "channelId1",
      }
    );
    expect(withinMeasurables()).toEqual(null);
    expect(() => withinMeasurables('a')).toThrow();
  });
});
