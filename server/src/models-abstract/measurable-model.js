const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

const { MEASURABLE_STATE } = require('../models/measurable-state');

class MeasurableModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Measurable,
      sequelize: models.sequelize,
    });
  }

  /**
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
   * @return {Promise<boolean>}
   */
  needsToBePending() {
    return this.model.needsToBePending();
  }

  /**
   * @param filter
   * @param pagination
   * @param restrictions
   * @return {Promise<void>}
   */
  async getAll(filter, pagination, restrictions) {
    const where = {};

    this.applyRestrictions(where, restrictions);

    // Filter
    if (_.isArray(options.states)) {
      where.state = { [this.in]: options.states };
    }
    if (filter.channelId) where.channelId = filter.channelId;
    if (filter.seriesId) where.seriesId = filter.seriesId;
    if (filter.creatorId) where.creatorId = filter.creatorId;
    where.isArchived = false;

    // Query
    return await this.models.Measurable.findAll({
      limit: pagination.limit,
      offset: pagination.offset,
      where,
      order: [
        [this.sequelize.col('stateOrder'), 'ASC'],
        ['createdAt', 'DESC'],
      ],
      attributes: {
        include: [
          this.getStateOrderField(),
        ]
      }
    });
  }
}

MeasurableModel.MEASURABLE_STATE = MEASURABLE_STATE;

module.exports = {
  MeasurableModel,
};
