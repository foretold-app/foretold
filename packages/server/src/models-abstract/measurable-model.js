const _ = require('lodash');

const models = require('../models');
const { MEASURABLE_STATE } = require('../models/enums/measurable-state');

const { ModelPostgres } = require('./model-postgres');
const { ResponseAll } = require('./classes/response-all');

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
  _getStateOrderField() {
    const {
      OPEN,
      JUDGEMENT_PENDING,
      JUDGED,
    } = MeasurableModel.MEASURABLE_STATE;

    return this.sequelize.literal(
      `(CASE WHEN "state"='${OPEN}' THEN 1 ` +
      `WHEN "state"='${JUDGEMENT_PENDING}' THEN 2 ` +
      `WHEN "state"='${JUDGED}' THEN 3 ` +
      `ELSE 5 END) AS "stateOrder"`,
    );
  }

  /**
   * @public
   * @return {Promise<Models.Measurable[]>}
   */
  async needsToBePending() {
    return this.model.findAll({
      where: {
        state: MEASURABLE_STATE.OPEN,
        [this.or]: [
          {
            expectedResolutionDate: {
              [this.lt]: this.fn('now'),
            },
          },
          { expectedResolutionDate: null },
        ],
      },
    });
  }

  /**
   * @public
   * @return {Promise<Models.Measurable[]>}
   */
  async needsResolutionResponse() {
    return this.model.findAll({
      where: {
        state: MEASURABLE_STATE.JUDGEMENT_PENDING,
        expectedResolutionDate: {
          [this.lt]: this.fn('now'),
        },
      },
    });
  }

  /**
   * @public
   * @todo: use getConnection
   * @deprecated
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Models.ObjectID} [filter.channelId]
   * @param {Models.ObjectID} [filter.seriesId]
   * @param {Models.ObjectID} [filter.creatorId]
   * @param {string[]} [filter.states]
   * @param {string[]} [filter.isArchived]
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [_options]
   * @return {Promise<{data: Models.Measurable[], total: number}>}
   */
  async getAll(filter = {}, pagination = {}, restrictions = {}, _options = {}) {
    const where = {};
    const include = [];

    if ('inspect' in filter) filter.inspect();
    if ('inspect' in pagination) pagination.inspect();
    if ('inspect' in restrictions) restrictions.inspect();

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

    const cond = { where, include };

    /** @type {number} */
    const total = await this.model.count(cond);
    const edgePagination = this._getPagination(pagination, total);

    const options = {
      ...cond,
      limit: edgePagination.limit,
      offset: edgePagination.offset,
      order: [
        [this.sequelize.col('stateOrder'), 'ASC'],
        ['createdAt', 'DESC'],
      ],
      attributes: {
        include: [this._getStateOrderField()],
      },
    };

    /** @type {Models.Measurable[]} */
    let data = await this.model.findAll(options);
    data = this._setIndexes(data, edgePagination);
    data.total = total;

    return new ResponseAll(data, total);
  }

  /**
   * @param {string} channelId
   * @return {Promise<*>}
   */
  async getOpenedCount(channelId) {
    return this.model.count({
      where: {
        channelId,
        state: MEASURABLE_STATE.OPEN,
      },
    });
  }
}

MeasurableModel.MEASURABLE_STATE = MEASURABLE_STATE;

module.exports = {
  MeasurableModel,
};
