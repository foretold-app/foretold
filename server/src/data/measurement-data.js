const models = require("../models");
const { notify } = require("../lib/notifications");

const { DataBase } = require('./data-base');

class MeasurementData extends DataBase {

  /**
   * @todo: rename
   * @param {object} data
   * @param {object} user
   * @return {Promise<*>}
   */
  async createOne(data, user) {
    const newMeasurement = await models.Measurement.create(data);
    const notification = await newMeasurement.creationNotification(user);
    notify(notification);
    return newMeasurement;
  }

  /**
   * @public
   * @param {object} options
   * @param {number} [options.offset]
   * @param {number} [options.limit]
   * @param {object} [options.agentId]
   * @return {Promise<*|Array<Model>>}
   */
  async getAll(options = {}) {
    return await models.Measurement.findAll({
      where: {
        measurableId: { $in: this.measurableIdsLiteral(options.agentId) },
      }
    });
  }

  /**
   * @param {string} id
   * @param {object} options
   * @param {object} [options.agentId]
   * @return {Promise<*>}
   */
  async getOne(id, options = {}) {
    return await models.Measurement.findOne({
      where: {
        id,
        measurableId: { $in: this.measurableIdsLiteral(options.agentId) },
      }
    });
  }
}

module.exports = {
  MeasurementData,
};
