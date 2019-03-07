const models = require("../models");
const { notify } = require("../lib/notifications");
const { getAuth0Id } = require('../helpers');
const { usersData } = require('./');

class MeasurementData {
  constructor() {
    this.usersData = usersData;
  }

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
    const _auth0Id = await this.usersData.getAuth0Id(options);
    const user = await this.usersData.auth0User(_auth0Id);
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
