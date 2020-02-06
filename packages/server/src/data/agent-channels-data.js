const _ = require('lodash');

const { DataBase } = require('./data-base');
const { AgentMeasurablesData } = require('./agent-measurables-data');
const { MeasurablesData } = require('./measurables-data');

const { Filter } = require('./classes');
const { Pagination } = require('./classes');
const { Options } = require('./classes');

const { AgentChannelModel } = require('../models');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentChannelModel} model
 */
class AgentChannelsData extends DataBase {
  constructor() {
    super();
    this.model = new AgentChannelModel();
    this.measurables = new MeasurablesData();
    this.agentMeasurables = new AgentMeasurablesData();
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoChannel(options),
    };
  }

  /**
   * Do not make any optimization here, it is early for this.
   * For each optimization we need to do a researching of the performance.
   * @param {Models.AgentID} agentId
   * @param {Models.ChannelID} channelId
   * @returns {Promise<number>}
   */
  async primaryPointScore(agentId, channelId) {

    const response = await this.model.sequelize.query(`
SELECT row_to_json(row (array_agg(
      row ("measurableId", "agentId", "createdAt", "channelId", "aggragationMeasurements", "objectiveMeasurements", "agentMeasurements")))),
      "channelId",
      "agentId"
FROM (
      SELECT "AgentMeasurables"."measurableId",
             "AgentMeasurables"."agentId",
             "Measurables"."createdAt",
             "Measurables"."channelId",
             (
               SELECT row_to_json(row (array_agg(row ("relevantAt", "value")))) as "aggragationMeasurements"
               FROM "Measurements"
               WHERE "measurableId" = "AgentMeasurables"."measurableId"
                 AND "competitorType" = 'AGGREGATION'
             ) as "aggragationMeasurements",
             (
               SELECT row_to_json(row (array_agg(row ("relevantAt", "value")))) as "objectiveMeasurements"
               FROM "Measurements"
               WHERE "measurableId" = "AgentMeasurables"."measurableId"
                 AND "competitorType" = 'OBJECTIVE'
               LIMIT 1
             ) as "objectiveMeasurements",
             (
               SELECT row_to_json(row (array_agg(row ("relevantAt", "value")))) as "objectiveMeasurements"
               FROM "Measurements"
               WHERE "measurableId" = "AgentMeasurables"."measurableId"
                 AND "agentId" = "AgentMeasurables"."agentId"
                 AND "competitorType" = 'COMPETITIVE'
             ) as "agentMeasurements"
      FROM "AgentMeasurables"
             LEFT JOIN "Measurables" ON "Measurables"."id" = "AgentMeasurables"."measurableId"
    ) as "foo"
WHERE "channelId" = '${channelId}' AND "agentId" = '${agentId}'
GROUP BY "channelId", "agentId"
`, { raw: true, type: this.model.sequelize.QueryTypes.SELECT });

    const primaryPointScore$ = response.map((item) => {
      return this.agentMeasurables.primaryPointScore2(agentId, item.row_to_json.f1);
    });

    const primaryPointScores = await Promise.all(primaryPointScore$);

    const sum = _.chain(primaryPointScores)
      .remove(_.isObject)
      .map((r) => r.score)
      .sum()
      .value();

    return sum;
  }

  /**
   * @param {Models.ChannelID} channelId
   * @returns {Promise<*>}
   */
  async _getMeasurables(channelId) {
    const filter = new Filter({ channelId });
    const pagination = new Pagination();
    const attributes = { fields: ['id'] };
    const options = new Options({ raw: true, attributes });
    return this.measurables.getAll(filter, pagination, options);
  }
}

module.exports = {
  AgentChannelsData,
};
