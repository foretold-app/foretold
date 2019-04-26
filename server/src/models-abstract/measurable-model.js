const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

const { MEASURABLE_STATE } = require('../models/measurable-state');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class MeasurableModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Measurable,
      sequelize: models.sequelize,
    });
  }

  /**
   * @protected
   * @return {Sequelize.literal|*}
   */
  getStateOrderField() {
    return this.sequelize.literal(
      `(CASE WHEN "state"='${MeasurableModel.MEASURABLE_STATE.OPEN}' THEN 1 ` +
      `WHEN "state"='${MeasurableModel.MEASURABLE_STATE.JUDGEMENT_PENDING}' THEN 2 ` +
      `WHEN "state"='${MeasurableModel.MEASURABLE_STATE.JUDGED}' THEN 3 ` +
      `ELSE 5 END) AS "stateOrder"`,
    );
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  needsToBePending() {
    return this.model.needsToBePending();
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {string} [filter.channelId]
   * @param {string} [filter.seriesId]
   * @param {string} [filter.creatorId]
   * @param {string[]} [filter.states]
   * @param {string[]} [filter.isArchived]
   * @param {boolean} [filter.isAggregated]
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @return {Promise<{data: Models.Measurable[], total: number}>}
   */
  async getAll(filter = {}, pagination, restrictions) {
    const where = {};
    const include = [];

    this.applyRestrictions(where, restrictions);
    this.applyRestrictionsIncluding(include, restrictions);
    this.applyFilter(where, filter);

    // Filter
    if (_.isArray(filter.states)) {
      where.state = { [this.in]: filter.states };
    }
    if (filter.channelId) where.channelId = filter.channelId;
    if (filter.seriesId) where.seriesId = filter.seriesId;
    if (filter.creatorId) where.creatorId = filter.creatorId;
    where.aggregatedAt = { [filter.isAggregated ? this.ne : this.eq]: null };

    const cond = { where, include };

    /** @type {number} */
    const total = await this.model.count(cond);
    const edgePagination = this.getPagination(pagination, total);

    const options = {
      ...cond,
      limit: edgePagination.limit,
      offset: edgePagination.offset,
      order: [
        [this.sequelize.col('stateOrder'), 'ASC'],
        ['createdAt', 'DESC'],
      ],
      attributes: {
        include: [this.getStateOrderField()],
      },
    };

    /** @type {Models.Measurable[]} */
    let data = await this.model.findAll(options);
    data = this.setIndexes(data, edgePagination);

    return { data, total };
  }
}

MeasurableModel.MEASURABLE_STATE = MEASURABLE_STATE;

module.exports = {
  MeasurableModel,
};
