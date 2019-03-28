const models = require("../models");
const { notify } = require("../lib/notifications");

const { DataBase } = require('./data-base');

class MeasurementData extends DataBase {

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
