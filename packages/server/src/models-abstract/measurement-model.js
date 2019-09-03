const _ = require('lodash');
const assert = require('assert');

const models = require('../models');
const {
  MEASUREMENT_COMPETITOR_TYPE,
} = require('../models/enums/measurement-competitor-type');
const { BrierScore } = require('../lib/brier-score');

const { ModelPostgres } = require('./model-postgres');

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
   * @param {Models.ObjectID} agentId
   * @param {string} [name]
   * @return {string}
   */
  _taggedMeasurementsLiteral(agentId, name = '') {
    return this.literal(this._taggedMeasurements(agentId, name));
  }

  /**
   * @protected
   * @param {Models.ObjectID} agentId
   * @param {string} name
   * @return {string}
   */
  _taggedMeasurements(agentId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* T͟a͟g͟g͟e͟d͟ ͟M͟e͟a͟s͟u͟r͟e͟m͟e͟n͟t͟s͟ (${ name }) */
      SELECT "taggedMeasurementId"
      FROM "Measurements"
      WHERE "agentId" = '${ agentId }'
      AND "taggedMeasurementId" IS NOT NULL
    )`;
  }

  /**
   * @param {string} agentId
   * @return {Promise<number>}
   */
  async getBrierScore(agentId) {
    const raw = await this.getBinaryPercentages(agentId);

    const brierScores = raw.map(item => {
      return new BrierScore(item.probabilities, item.questionResult).mean();
    });

    const mean = _.mean(brierScores);
    return _.round(mean, 2);
  }

  /**
   * @public
   * @param {Models.ObjectID} agentId
   * @return {Promise.<{
   *   measurableId: string,
   *   agentId: string,
   *   probabilities: number[],
   *   questionResult: boolean
   * }[]>}
   */
  async getBinaryPercentages(agentId) {
    const query = this._binaryPercentages(agentId);
    const response = await this.sequelize.query(query);
    const result = _.head(response);
    return result.map((item) => {
      item.probabilities = item.probabilities.map(_.toNumber);
      item.questionResult = item.questionResult === 'true';
      return item;
    });
  }

  /**
   * @todo: see this._publicAndJoinedChannels()
   * @protected
   * @param {Models.ObjectID} [agentId]
   * @return {string}
   */
  _binaryPercentages(agentId) {
    assert(!!agentId, 'Agent ID is required.');

    const agentMeasurements =
      this._agentMeasurementsJudgedPercentageCompetitive(agentId);

    return `(
      /* B͟i͟n͟a͟r͟y͟ ͟P͟e͟r͟c͟e͟n͟t͟a͟g͟e͟s͟ */
      WITH "AgentMeasurements" AS ${ agentMeasurements }
      SELECT 
        "AgentMeasurements".*, 
        "Measurements"."value" ->> 'data' as "questionResult"
      FROM "AgentMeasurements"
               LEFT JOIN "Measurements"
                         ON "Measurements"."measurableId" =
                            "AgentMeasurements"."measurableId" AND
                            "Measurements"."competitorType" = 'OBJECTIVE' AND
                            "Measurements"."value" ->> 'dataType' = 'binary'
    )`;
  }

  /**
   * @todo: see this._publicAndJoinedChannels()
   * @protected
   * @param {Models.ObjectID} agentId
   * @return {string}
   */
  _agentMeasurementsJudgedPercentageCompetitive(agentId) {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* R͟e͟t͟u͟r͟n͟s͟ ͟a͟r͟r͟a͟y͟s͟ ͟o͟f͟ ͟p͟r͟e͟d͟i͟c͟t͟i͟o͟n͟s͟ */
      SELECT "Measurements"."measurableId",
             "Measurements"."agentId",
             array_agg("Measurements"."value" ->> 'data') as "probabilities"
      FROM "Measurements"
               LEFT JOIN "Measurables"
                         ON "Measurements"."measurableId" = "Measurables".id
      WHERE "Measurables"."state" = 'JUDGED'
        AND "Measurables"."valueType" = 'PERCENTAGE'
        AND "Measurements"."competitorType" = 'COMPETITIVE'
        AND "Measurements"."agentId" = '${ agentId }'
      GROUP BY "Measurements"."measurableId", "Measurements"."agentId"
    )`;
  }

  /**
   * @return {*[] | null}
   * @private
   */
  _getOrder() {
    return [['relevantAt', 'DESC']];
  }

  /**
   * @public
   * @param {Models.ObjectID} measurableId
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
    });
  }

  /**
   * @public
   * @param {Models.ObjectID} measurableId
   * @param {Date} relevantAt
   * @returns {Promise<Model>}
   */
  async getPreviousRelevantAggregate(measurableId, relevantAt) {
    assert(!!measurableId, 'Measurable ID is required.');
    assert(!!relevantAt, 'RelevantAt is required.');
    return this.model.findOne({
      where: {
        measurableId,
        competitorType: MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
        relevantAt: { [this.lt]: relevantAt },
      },
      order: [
        ['relevantAt', 'DESC'],
      ],
    });
  }

  /**
   * @public
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<Model>}
   */
  async getLatestAggregate(measurableId) {
    assert(!!measurableId, 'Measurable ID is required.');
    return this.model.findOne({
      where: {
        measurableId,
        competitorType: MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
      },
      order: [
        ['relevantAt', 'DESC'],
      ],
    });
  }
}

module.exports = {
  MeasurementModel,
};
