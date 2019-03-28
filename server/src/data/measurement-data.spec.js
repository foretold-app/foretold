const models = require('../models');
const { MeasurementData } = require('./measurement-data');

describe('tests Measurement Data layer', () => {

  it('class should be a constructor', () => {
    expect(MeasurementData).toBeInstanceOf(Function);
  });

  const instance = new MeasurementData();

  describe('getOne()', () => {
    const id = 'id2';
    const options = { agentId: 'agentId1' };
    beforeEach(() => {
      jest.spyOn(models.Measurement, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(instance, 'measurableIdsLiteral').mockReturnValue(
        'measurableIdsLiteral',
      );
    });
    it('finds a measurement', () => {
      return instance.getOne(id, options).then((result) => {
        expect(models.Measurement.findOne).toHaveBeenCalledWith({
          "where": {
            "id": "id2",
            "measurableId": { "$in": "measurableIdsLiteral" },
          }
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getAll()', () => {
    const options = { agentId: 'agentId1' };
    beforeEach(() => {
      jest.spyOn(models.Measurement, 'findAll').mockReturnValue(
        Promise.resolve(true),
      );
      jest.spyOn(instance, 'measurableIdsLiteral').mockReturnValue(
        'measurableIdsLiteral',
      );
    });
    it('finds a measurement', () => {
      return instance.getAll(options).then((result) => {
        expect(models.Measurement.findAll).toHaveBeenCalledWith({
          "where": { "measurableId": { "$in": "measurableIdsLiteral" } }
        });
        expect(result).toBe(true);
      });
    });
  });


});
