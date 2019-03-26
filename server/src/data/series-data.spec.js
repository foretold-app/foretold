const models = require('../models');
const { SeriesData } = require('./series-data');

describe('SeriesData', () => {

  it('class should be a constructor', () => {
    expect(SeriesData).toBeInstanceOf(Function);
  });

  const instance = new SeriesData();

  describe('createSeries()', () => {
    const root = {};
    const args = {};
    const context = { user: { agentId: 'agentId1' } };
    beforeEach(() => {
      jest.spyOn(models.Series, 'create').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('creates series', () => {
      return instance.createSeries(root, args, context).then((result) => {
        expect(models.Series.create).toHaveBeenCalledWith({
          "creatorId": "agentId1",
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getOne()', () => {
    const id = 'id3';
    beforeEach(() => {
      jest.spyOn(models.Series, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds series', () => {
      return instance.getOne(id).then((result) => {
        expect(models.Series.findOne).toHaveBeenCalledWith({
          "where": { "id": "id3" },
        });
        expect(result).toBe(true);
      });
    });
  });


});
