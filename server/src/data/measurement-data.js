const models = require("../models");
const { notify } = require("../lib/notifications");

class MeasurementData {

  constructor() { }

  /**
   * @todo: rename
   * @param root
   * @param values
   * @param options
   * @return {Promise<*>}
   */
  async createMeasurement(root, values, options) {
    const {
      value,
      competitorType,
      measurableId,
      description,
    } = values;
    const user = options.user;
    const newMeasurement = await models.Measurement.create({
      value,
      competitorType,
      measurableId,
      description,
      agentId: user.agentId,
    });
    const notification = await newMeasurement.creationNotification(user);
    notify(notification);
    const measurable = await newMeasurement.getMeasurable();
    return newMeasurement;
  }

}

module.exports = {
  MeasurementData,
};
