const models = require('../models');
const { SeriesData } = require('./series-data');

describe('SeriesData', () => {

  it('class should be a constructor', () => {
    expect(SeriesData).toBeInstanceOf(Function);
  });

  const instance = new SeriesData();

  describe('getOne()', () => {
    const id = 'id3';
    const options = { agentId: 'agentId1' };
    beforeEach(() => {
      jest.spyOn(models.Series, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds series', () => {
      return instance.getOne(id, options).then((result) => {
        expect(models.Series.findOne).toHaveBeenCalledWith({
          "where": {
            "channelId": {
              [instance.model.Op.in]: "channelIdsLiteral",
            },
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
    });
    it('finds all series', () => {
      return instance.getAll(options).then((result) => {
        expect(models.Series.findAll).toHaveBeenCalledWith({
          "where": {
            "channelId": {
              [instance.model.Op.in]: "channelIdsLiteral"
            }
          },
        });
        expect(result).toBe(true);
      });
    });
  });


});
