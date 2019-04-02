const models = require('../models');
const { SeriesData } = require('./series-data');

describe('SeriesData', () => {

  it('class should be a constructor', () => {
    expect(SeriesData).toBeInstanceOf(Function);
  });

  const instance = new SeriesData();

  describe('createOne()', () => {
    const data = {};
    beforeEach(() => {
      jest.spyOn(models.Series, 'create').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates series', () => {
      return instance.createOne(data).then((result) => {
        expect(models.Series.create).toHaveBeenCalledWith(data);
        expect(result).toBe(true);
      });
    });
  });

  describe('getOne()', () => {
    const id = 'id3';
    const options = { agentId: 'agentId1' };
    beforeEach(() => {
      jest.spyOn(models.Series, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(instance, 'channelIdsLiteral').mockReturnValue(
        'channelIdsLiteral',
      );
    });
    it('finds series', () => {
      return instance.getOne(id, options).then((result) => {
        expect(models.Series.findOne).toHaveBeenCalledWith({
          "where": {
            "channelId": { "$in": "channelIdsLiteral" },
            "id": "id3",
          }
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getAll()', () => {
    const options = { agentId: 'agentId1' };
    beforeEach(() => {
      jest.spyOn(models.Series, 'findAll').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(instance, 'channelIdsLiteral').mockReturnValue(
        'channelIdsLiteral',
      );
    });
    it('finds all series', () => {
      return instance.getAll(options).then((result) => {
        expect(models.Series.findAll).toHaveBeenCalledWith({
          "where": { "channelId": { "$in": "channelIdsLiteral" } },
        });
        expect(result).toBe(true);
      });
    });
  });


});
