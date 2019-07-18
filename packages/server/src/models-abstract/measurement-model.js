const _ = require('lodash');

const models = require('../models');
const { splitBy } = require('../lib/functions');

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
   * @return {string}
   */
  _taggedMeasurementsLiteral(agentId) {
    return this.literal(this._taggedMeasurements(agentId));
  }

  /**
   * @todo: see this._channelIds()
   * @protected
   * @param {Models.ObjectID} agentId
   * @return {string}
   */
  _taggedMeasurements(agentId) {
    return `(
      SELECT "taggedMeasurementId"
      FROM "Measurements"
      WHERE "agentId" = '${agentId}'
      AND "taggedMeasurementId" IS NOT NULL
    )`;
  }

  async getBrierScore(agentId) {
    const raw = await this.getBinaryPercentages(agentId);

  }

  /**
   * @public
   * @param {Models.ObjectID} agentId
   * @return {Promise.<{
   *   measurableId: string,
   *   agentId: string,
   *   datas: number[],
   *   data: string
   * }[]>}
   */
  async getBinaryPercentages(agentId) {
    const query = this._binaryPercentages(agentId);
    const result = await this.sequelize.query(query);
    return _.head(result);
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
      WITH "AgentMeasurements" AS ${agentMeasurements}
      SELECT "AgentMeasurements".*, "Measurements"."value" ->> 'data' as "data"
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
      SELECT "Measurements"."measurableId",
             "Measurements"."agentId",
             array_agg("Measurements"."value" ->> 'data') as "datas"
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

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Models.ObjectID} [filter.measurableId]
   * @param {Models.ObjectID} [filter.agentId]
   * @param {string[]} [filter.competitorType]
   * @param {Models.ObjectID} [filter.notTaggedByAgent]
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [_options]
   * @return {Promise<{data: Models.Measurement[], total: number}>}
   */
  async getAll(filter = {}, pagination = {}, restrictions = {}, _options = {}) {
    const { where, include, spacedLimit } = this.makeFilter(filter);
    this.applyRestrictions(where, restrictions);

    /** @type {number} */
    const total = await this.model.count({ where, include });
    const edgePagination = this.getPagination(pagination, total);

    const cond = {
      limit: edgePagination.limit,
      offset: edgePagination.offset,
      order: [['relevantAt', 'DESC']],
      where,
      include,
    };

    /** @type {Models.Measurement[]} */
    let data = await this.model.findAll(cond);
    data = this.setIndexes(data, edgePagination);
    data.total = total;

    // tricky
    if (spacedLimit) data = splitBy(data, spacedLimit);

    return { data, total };
  }

  /**
   * @protected
   * @param {object} filter
   * @return {{include: Array, spacedLimit: *, where: {}}}
   */
  makeFilter(filter = {}) {
    const where = { [this.and]: [] };
    const include = [];

    const startDate = _.get(filter, 'findInDateRange.startDate');
    const endDate = _.get(filter, 'findInDateRange.endDate');
    const spacedLimit = _.get(filter, 'findInDateRange.spacedLimit');

    if (filter.measurableId) where.measurableId = filter.measurableId;
    if (filter.agentId) where.agentId = filter.agentId;
    if (filter.competitorType) where.competitorType = {
      [this.in]: filter.competitorType,
    };
    if (startDate) where[this.and].push({
      createdAt: { [this.gte]: startDate },
    });
    if (endDate) where[this.and].push({ createdAt: { [this.lte]: endDate } });
    if (filter.notTaggedByAgent) where.id = {
      [this.notIn]: this._taggedMeasurementsLiteral(filter.notTaggedByAgent),
    };

    return { where, include, spacedLimit };
  }
}

module.exports = {
  MeasurementModel,
};
