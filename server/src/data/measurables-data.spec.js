const models = require('../models');
const { MeasurablesData } = require('./measurables-data');

describe('tests Measurables Data layer', () => {

  it('class should be a constructor', () => {
    expect(MeasurablesData).toBeInstanceOf(Function);
  });

  const instance = new MeasurablesData();

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
          "where": {
            "channelId": {
              [models.sequelize.Op.in]: "channelIdsLiteral"
            },
            "id": "id1"
          }
        });
        expect(result).toBe(true);
      });
    });
  });

});
