const { notify } = require('../lib/notifications');

const { DataBase } = require('./data-base');

const { MeasurementModel } = require('../models-abstract');
const { MEASURABLE_STATE } = require('../enums/measurable-state');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../enums/measurement-competitor-type');

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
   * @todo: move logic
   * @todo: fix interface
   * @todo: rename, move down
   * @public
   * @deprecated: use createOne
   * @param {object} [data]
   * @param {Models.Creator} creator
   * @return {Promise<Models.Measurement>}
   */
  async createOne(data = {}, creator) {
    const measurement = await super.createOne(data);

    const notification = await measurement.getCreationNotification(creator);
    const measurable = await measurement.getMeasurable();
    const channel = await measurable.getChannel();
    if (channel.isPublic) {
      await notify(notification);
    }

    return measurement;
  }

  /**
   * @todo: move it into Model Abstraction layer
   * @public
   * @param {Models.Measurable} measurable
   * @param {Models.ObjectID | null} agentId
   * @return {Promise<Models.Model>}
   */
  async getLatest({ measurable, agentId } = {}) {
    const measurableId = measurable.id;
    const competitorType = MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE;

    if (measurable.state === MEASURABLE_STATE.JUDGED) {
      const measurement = await this.model.getOne({
        measurableId,
        agentId,
        competitorType,
      });
      if (!measurement) {
        throw new Error('Measurement as Objective is not found');
      }
      return measurement;
    }

    return this.model.getOne({ measurableId, agentId });
  }

  /**
   * @public
   * @param {Models.ObjectID | null} agentId
   * @return {Promise<number>}
   */
  async getBrierScore(agentId) {
    return this.model.getBrierScore(agentId);
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      measurableId: true,
      isAdmin: options.isAdmin,
      agentId: options.agentId,
    };
  }

  /**
   * @public
   * @param {Models.ObjectID | null} measurableId
   * @returns {Promise<Model>}
   */
  async getOutcome(measurableId) {
    return this.model.getOutcome(measurableId);
  }

  /**
   * @public
   * @param {Models.ObjectID | null} measurableId
   * @param {Models.ObjectID | null} agentId
   * @param {Date} relevantAt
   * @returns {Promise<Model>}
   */
  async getPreviousRelevantAggregate(measurableId, agentId, relevantAt) {
    return this.model.getPreviousRelevantAggregate(
      measurableId,
      agentId,
      relevantAt,
    );
  }

  /**
   * @public
   * @param {Models.ObjectID | null} measurableId
   * @param {Models.ObjectID | null} agentId
   * @returns {Promise<Model>}
   */
  async getLatestAggregate(measurableId, agentId) {
    return this.model.getLatestAggregate(measurableId, agentId);
  }
}

module.exports = {
  MeasurementsData,
};
