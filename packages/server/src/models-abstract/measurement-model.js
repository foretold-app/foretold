const _ = require('lodash');

const models = require('../models');
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
   * @todo: see this._channelIds()
   * @param {Models.ObjectID} [agentId]
   * @param {string} [name]
   * @return {string}
   */
  _taggedMeasurementsLiteral(agentId, name = '') {
    return this.literal(this._taggedMeasurements(agentId, name));
  }

  /**
   * @todo: see this._channelIds()
   * @protected
   * @param {Models.ObjectID} agentId
   * @param {string} name
   * @return {string}
   */
  _taggedMeasurements(agentId, name = '') {
    return `(
      /* T͟a͟g͟g͟e͟d͟ ͟M͟e͟a͟s͟u͟r͟e͟m͟e͟n͟t͟s͟ ͟O͟n͟l͟y͟ (${name}) */
      SELECT "taggedMeasurementId"
      FROM "Measurements"
      WHERE "agentId" = '${agentId}'
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
   * @todo: see this._channelIds()
   * @protected
   * @param {Models.ObjectID} [agentId]
   * @return {string}
   */
  _binaryPercentages(agentId) {
    const agentMeasurements =
      this._agentMeasurementsJudgedPercentageCompetitive(agentId);

    return `(
      /* B͟i͟n͟a͟r͟y͟ ͟P͟e͟r͟c͟e͟n͟t͟a͟g͟e͟s͟ */
      WITH "AgentMeasurements" AS ${agentMeasurements}
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
   * @todo: see this._channelIds()
   * @protected
   * @param {Models.ObjectID} agentId
   * @return {string}
   */
  _agentMeasurementsJudgedPercentageCompetitive(agentId) {
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
        AND "Measurements"."agentId" = '${agentId}'
      GROUP BY "Measurements"."measurableId", "Measurements"."agentId"
    )`;
  }
}

module.exports = {
  MeasurementModel,
};
