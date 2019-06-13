const { notify } = require("../lib/notifications");

const { DataBase } = require('./data-base');

const { MeasurementModel } = require('../models-abstract');
const { MEASURABLE_STATE } = require('../models/measurable-state');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../models/measurement-competitor-type');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {MeasurementModel} MeasurementModel
 */
class MeasurementsData extends DataBase {

  constructor() {
    super();
    this.MeasurementModel = new MeasurementModel();
    this.model = this.MeasurementModel;
  }

  /**
   * @todo: fix interface
   * @todo: rename, move down
   * @param {object} data
   * @param {Models.Creator} creator
   * @return {Promise<Models.Measurement>}
   */
  async createOne(data, creator) {
    const measurement = await this.models.Measurement.create(data);

    const notification = await measurement.getCreationNotification(creator);
    notify(notification);

    return measurement;
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   * @param {Models.ObjectID} [options.measurableId]
   * @param {Models.ObjectID} options.agentId
   * @return {Promise<{data: Models.Measurement[], total: number}>}
   */
  async getAll(filter = {}, pagination = {}, options = {}) {
    const restrictions = {
      measurableId: true,
      isAdmin: options.isAdmin,
      agentId: options.agentId,
    };
    return await this.MeasurementModel.getAll(filter, pagination, restrictions);
  }

  /**
   * @todo: fix interface
   * @todo: move down
   * @param {Models.ObjectID} id
   * @param {object} options
   * @param {string} [options.agentId]
   * @return {Promise<*>}
   */
  async getOne(id, options = {}) {
    return await this.models.Measurement.findOne({
      where: {
        id,
        measurableId: {
          [this.model.Op.in]: this.MeasurementModel.measurableIdsLiteral(options.agentId)
        },
      }
    });
  }

  /**
   * @param {Models.Measurable} measurable
   * @param {Models.ObjectID} agentId
   * @return {Promise<Models.Measurement>}
   */
  async getLatest({ measurable, agentId } = {}) {
    const measurableId = measurable.id;
    const competitorType = MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE;

    if (measurable.state === MEASURABLE_STATE.JUDGED) {
      const measurement = await this.MeasurementModel.getOne({
        measurableId,
        agentId,
        competitorType,
      });
      if (!measurement) {
        throw new Error('Measurement as Objective is not found');
      }
      return measurement;
    }

    return await this.MeasurementModel.getOne({ measurableId, agentId });
  }
}

module.exports = {
  MeasurementsData,
};
