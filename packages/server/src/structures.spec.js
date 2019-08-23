const index = require('./structures');
const { withinMeasurables } = require('./structures');
const { withinPublicChannelsById } = require('./structures');
const { withinPublicChannelsByChannelId } = require('./structures');
const { withinPublicAndJoinedChannelsById } = require('./structures');
const { withinPublicAndJoinedChannelsByChannelId } = require('./structures');
const { withinJoinedChannelsById } = require('./structures');
const { withinJoinedChannelsByChannelId } = require('./structures');

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
  it('withinPublicChannelsById()', () => {
    expect(withinPublicChannelsById()).toEqual({as: 'id'});
  });
  it('withinPublicChannelsByChannelId()', () => {
    expect(withinPublicChannelsByChannelId()).toEqual({as: 'channelId'});
  });
  it('withinPublicAndJoinedChannelsById()', () => {
    expect(() => withinPublicAndJoinedChannelsById()).toThrow();
    expect(withinPublicAndJoinedChannelsById('a')).toEqual({
      as: 'id', agentId: 'a',
    });
  });
  it('withinPublicAndJoinedChannelsByChannelId()', () => {
    expect(() => withinPublicAndJoinedChannelsByChannelId()).toThrow();
    expect(withinPublicAndJoinedChannelsByChannelId('a')).toEqual({
      as: 'channelId', agentId: 'a',
    });
  });
  it('withinJoinedChannelsById()', () => {
    expect(() => withinJoinedChannelsById()).toThrow();
    expect(withinJoinedChannelsById('a')).toEqual({
      as: 'id', agentId: 'a',
    });
  });
  it('withinJoinedChannelsByChannelId()', () => {
    expect(() => withinJoinedChannelsByChannelId()).toThrow();
    expect(withinJoinedChannelsByChannelId('a')).toEqual({
      as: 'channelId', agentId: 'a',
    });
  });
});
