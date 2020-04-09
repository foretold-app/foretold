const assert = require('assert');

const models = require('./definitions');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../enums');

const { ModelPostgres } = require('./model-postgres');

const { Params } = require('../classes');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class MeasurementModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Measurement,
      sequelize: models.sequelize,
    });
  }

  /**
   * @return {*[] | null}
   * @protected
   */
  _getOrder() {
    return [
      ['relevantAt', 'DESC'],
      ['createdAt', 'DESC'],
    ];
  }

  /**
   * @public
   * @todo: To use "applyFilter" to use "transactions" later!
   * @param {Defs.MeasurableID | null} measurableId
   * @returns {Promise<Model>}
   */
  async getOutcome(measurableId) {
    assert(!!measurableId, 'Measurable ID is required.');

    return this.model.findOne({
      where: {
        measurableId,
        competitorType: {
          [this.in]: [
            MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
            MEASUREMENT_COMPETITOR_TYPE.UNRESOLVED,
          ],
        },
      },
      order: [
        ['createdAt', 'DESC'],
      ],
      raw: true,
    });
  }

  /**
   * @public
   * @todo: To use "applyFilter" to use "transactions" later!
   * @param {Defs.MeasurableID | null} measurableId
   * @param {Defs.AgentID | null} agentId
   * @param {Date} relevantAt
   * @returns {Promise<Model>}
   */
  async getPreviousRelevantAggregate(measurableId, agentId, relevantAt) {
    assert(!!measurableId, 'Measurable ID is required.');
    assert(!!relevantAt, 'RelevantAt is required.');

    return this.model.findOne({
      where: {
        agentId,
        measurableId,
        competitorType: MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
        relevantAt: { [this.lt]: relevantAt },
      },
      order: [
        ['relevantAt', 'DESC'],
      ],
      raw: true,
    });
  }

  /**
   * @public
   * @todo: To use "applyFilter" to use "transactions" later.
   * @param {Defs.MeasurableID | null} measurableId
   * @param {Defs.AgentID | null} agentId
   * @returns {Promise<Model>}
   */
  async getLatestAggregate(measurableId, agentId) {
    assert(!!measurableId, 'Measurable ID is required.');

    return this.model.findOne({
      where: {
        agentId,
        measurableId,
        competitorType: MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
      },
      order: [
        ['relevantAt', 'DESC'],
      ],
    });
  }

  /**
   * @public
   * @todo: To use "applyFilter" to use "transactions" later.
   * @param {Defs.Measurable} measurable
   * @param {Defs.AgentID | null} agentId
   * @return {Promise<Models.Definition>}
   */
  async getLatest({ measurable, agentId } = {}) {
    const measurableId = measurable.id;
    const params = new Params({ measurableId, agentId });
    return this.getOne(params);
  }
}

module.exports = {
  MeasurementModel,
};
