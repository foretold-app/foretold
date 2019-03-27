const models = require('../models');
const { MeasurablesData } = require('./measurables-data');

describe('tests Measurables Data layer', () => {

  it('class should be a constructor', () => {
    expect(MeasurablesData).toBeInstanceOf(Function);
  });

  const instance = new MeasurablesData();

  describe('getAll()', () => {
    const options = {
      offset: 1,
      limit: 2,
      channelId: '3',
      seriesId: '4',
      creatorId: '5',
      agentId: 'agentId1'
    };
    beforeEach(() => {
      jest.spyOn(models.Measurable, 'findAll').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(instance, 'channelIdsLiteral').mockReturnValue(
        'channelIdsLiteral',
      );
    });
    it('finds all measurables', () => {
      return instance.getAll(options).then((result) => {
        expect(models.Measurable.findAll).toHaveBeenCalledWith({
          "limit": 2,
          "offset": 1,
          "order": [["createdAt", "DESC"]],
          "where": {
            "$and": [
              { "channelId": { "$in": "channelIdsLiteral" } },
              { "channelId": "3" }
            ],
            "creatorId": { "undefined": "5" },
            "seriesId": { "undefined": "4" },
            "state": { "neop": "ARCHIVED" }
          }
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getOne()', () => {
    const id = 'id1';
    const options = { agentId: 'agentId1' };
    beforeEach(() => {
      jest.spyOn(models.Measurable, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(instance, 'channelIdsLiteral').mockReturnValue(
        'channelIdsLiteral',
      );
    });
    it('finds a measurable', () => {
      return instance.getOne(id, options).then((result) => {
        expect(models.Measurable.findOne).toHaveBeenCalledWith({
          "where": { "channelId": { "$in": "channelIdsLiteral" }, "id": "id1" }
        });
        expect(result).toBe(true);
      });
    });
  });

});
