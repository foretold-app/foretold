const assert = require('assert');

const models = require('./definitions');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../enums');

const { ModelPostgres } = require('./model-postgres');

const { Params } = require('../data/classes');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class MeasurementModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Measurement,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {string} [name]
   * @return {string}
   */
  _taggedMeasurementsLiteral(agentId, name = '') {
    return this.literal(this._taggedMeasurements(agentId, name));
  }

  /**
   * @protected
   * @param {Models.AgentID} agentId
   * @param {string} name
   * @return {string}
   */
  _taggedMeasurements(agentId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* T͟a͟g͟g͟e͟d͟ ͟M͟e͟a͟s͟u͟r͟e͟m͟e͟n͟t͟s͟ (${name}) */
      SELECT "taggedMeasurementId"
      FROM "Measurements"
      WHERE "agentId" = '${agentId}'
      AND "taggedMeasurementId" IS NOT NULL
    )`;
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
   * @param {Models.MeasurableID | null} measurableId
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
   * @param {Models.MeasurableID | null} measurableId
   * @param {Models.AgentID | null} agentId
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
   * @param {Models.MeasurableID | null} measurableId
   * @param {Models.AgentID | null} agentId
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
   * @param {Models.Measurable} measurable
   * @param {Models.AgentID | null} agentId
   * @return {Promise<Models.Model>}
   */
  async getLatest({ measurable, agentId } = {}) {
    const measurableId = measurable.id;
    return this.getOne(new Params({ measurableId, agentId }));
  }
}

module.exports = {
  MeasurementModel,
};
