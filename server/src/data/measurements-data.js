const { notify } = require("../lib/notifications");

const { DataBase } = require('./data-base');

const { MeasurementModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class MeasurementsData extends DataBase {

  constructor() {
    super();
    this.MeasurementModel = new MeasurementModel();
  }

  /**
   * @todo: rename
   * @param {object} data
   * @param {Models.Creator} creator
   * @return {Promise<Models.Measurement>}
   */
  async createOne(data, creator) {
    const measurement = await this.models.Measurement.create(data);
    /** @type {Models.Measurement} */
    const notification = await measurement.creationNotification(creator);
    notify(notification);
    return measurement;
  }

  /**
   * @public
   * @param {object} options
   * @param {number} [options.offset]
   * @param {number} [options.limit]
   * @param {string} [options.agentId]
   * @return {Promise<*|Array<Model>>}
   */
  async getAll(options = {}) {
    return await this.models.Measurement.findAll({
      where: {
        measurableId: {
          [this.models.sequelize.Op.in]: this.MeasurementModel.measurableIdsLiteral(options.agentId)
        },
      }
    });
  }

  /**
   * @param {string} id
   * @param {object} options
   * @param {string} [options.agentId]
   * @return {Promise<*>}
   */
  async getOne(id, options = {}) {
    return await this.models.Measurement.findOne({
      where: {
        id,
        measurableId: {
          [this.models.sequelize.Op.in]: this.MeasurementModel.measurableIdsLiteral(options.agentId)
        },
      }
    });
  }
}

module.exports = {
  MeasurementsData,
};
