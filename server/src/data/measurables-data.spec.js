const models = require('../models');
const { MeasurablesData } = require('./measurables-data');

describe('MeasurablesData', () => {
  afterEach(() => {
    jest.clearAllMocks();
    jest.restoreAllMocks();
  });

  it('class should be a constructor', () => {
    expect(MeasurablesData).toBeInstanceOf(Function);
  });

  const instance = new MeasurablesData();

  describe('getAll', () => {
    const options = {
      offset: 1,
      limit: 2,
      channelId: '3',
      seriesId: '4',
      creatorId: '5'
    };
    beforeAll(() => {
      jest.spyOn(models.Measurable, 'findAll').mockReturnValue(Promise.resolve(true));
    });
    it('getAll', () => {
      return instance.getAll(options).then((result) => {
        expect(models.Measurable.findAll).toHaveBeenCalledWith({
          "limit": 2,
          "offset": 1,
          "order": [["createdAt", "DESC"]],
          "where": {
            "channelId": { "undefined": "3" },
            "creatorId": { "undefined": "5" },
            "seriesId": { "undefined": "4" },
            "state": { "neop": "ARCHIVED" }
          }
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getOne', () => {
    const id = 'id1';
    beforeAll(() => {
      jest.spyOn(models.Measurable, 'findOne').mockReturnValue(Promise.resolve(true));
    });
    it('getOne', () => {
      return instance.getOne(id).then((result) => {
        expect(models.Measurable.findOne).toHaveBeenCalledWith({ "where": { "id": "id1" } });
        expect(result).toBe(true);
      });
    });
  });


});
