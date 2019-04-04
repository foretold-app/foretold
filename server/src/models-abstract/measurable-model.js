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
    this.Op = this.sequelize.Op;
    this.fn = this.sequelize.fn;
  }

  /**
   * @return {Sequelize.literal|*}
   */
  getStateOrderField() {
    return this.sequelize.literal(
      `(CASE WHEN "state"='${MeasurableModel.MEASURABLE_STATE.OPEN}' THEN 1 ` +
      `WHEN "state"='${MeasurableModel.MEASURABLE_STATE.JUDGEMENT_PENDING}' THEN 2 ` +
      `WHEN "state"='${MeasurableModel.MEASURABLE_STATE.JUDGED}' THEN 3 ` +
      `WHEN "state"='${MeasurableModel.MEASURABLE_STATE.ARCHIVED}' THEN 4 ` +
      `ELSE 5 END) AS "stateOrder"`,
    );
  }

  /**
   *
   * @param {object} filter
   * @param {number} filter.offset
   * @param {number} filter.limit
   * @param pagination
   * @param restrictions
   * @return {Promise<Models.Measurable[]>}
   */
  getAll(filter, pagination, restrictions) {
    const offset = _.get(pagination, 'offset', 10);
    const limit = _.get(pagination, 'limit', 0);
    return this.model.findAll({
      limit,
      offset,
      where: filter,
    });
  }

  /**
   * @return {Promise<Models.Measurable[]>}
   */
  getAllJudgementPendingNext() {
    return this.model.findAll({
      where: {
        state: MEASURABLE_STATE.OPEN,
        expectedResolutionDate: {
          [this.Op.lt]: this.fn('now'),
        }
      },
      limit: 10,
      offset: 0,
    });
  }
}

MeasurableModel.MEASURABLE_STATE = MEASURABLE_STATE;

module.exports = {
  MeasurableModel,
};
