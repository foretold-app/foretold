const data = require('../../data');
const { MEASURABLE_VALUE_TYPE } = require('../../enums/measurable-value-type');

class Creators {
  constructor() {
    this.data = data;
  }

  /**
   * @param {Models.Series} series
   * @returns {Promise<void>}
   */
  async createMeasurables(series) {
    for (const subject of series.subjects) {
      for (const property of series.properties) {
        for (const date of series.dates) {
          console.log(
            'Making Measurable for Series:',
            subject,
            property,
            date,
          );
          await this.data.measurables.createOne({
            name: '',
            labelSubject: subject,
            labelProperty: property,
            labelOnDate: date,
            expectedResolutionDate: date,
            seriesId: series.id,
            creatorId: series.creatorId,
            channelId: series.channelId,
            valueType: MEASURABLE_VALUE_TYPE.FLOAT,
          });
        }
      }
    }
  };
}

module.exports = {
  Creators,
};
