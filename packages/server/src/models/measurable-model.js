const models = require('./definitions');
const { MEASURABLE_STATE } = require('../enums');

const { ModelPostgres } = require('./model-postgres');

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
      CLOSED_AS_UNRESOLVED,
    } = MeasurableModel.MEASURABLE_STATE;

    return this.sequelize.literal(
      `(CASE WHEN "state"='${OPEN}' THEN 1 `
      + `WHEN "state"='${JUDGEMENT_PENDING}' THEN 2 `
      + `WHEN "state"='${JUDGED}' THEN 3 `
      + `WHEN "state"='${CLOSED_AS_UNRESOLVED}' THEN 3 `
      + 'ELSE 5 END) AS "stateOrder"',
    );
  }

  /**
   * @public
   * @todo: To use "applyFilter" to use "transactions" later.
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

  /**
   * @protected
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Models.AgentID} [filter.userId]
   */
  applyFilter(where = {}, filter = {}) {
    super.applyFilter(where, filter);

    if (!!filter.needsToBePending) {
      where[this.and].push({
        state: MEASURABLE_STATE.OPEN,
        [this.or]: [
          {
            expectedResolutionDate: {
              [this.lt]: this.fn('now'),
            },
          },
          {
            expectedResolutionDate: null,
          },
        ],
      });
    }

    if (!!filter.needsResolutionResponse) {
      where[this.and].push({
        state: MEASURABLE_STATE.JUDGEMENT_PENDING,
        resolutionEndpoint: {
          [this.and]: [
            { [this.not]: null },
            { [this.not]: '' },
          ],
        },
        expectedResolutionDate: {
          [this.lt]: this.fn('now'),
        },
      });
    }

    return where;
  }

  /**
   * @return {*[]}
   * @private
   */
  _getOrder() {
    return [
      [this.sequelize.col('stateOrder'), 'ASC'],
      ['createdAt', 'DESC'],
    ];
  }

  /**
   *
   * @return {{include: Sequelize.literal|*[]}}
   * @private
   */
  _getAttributes() {
    return {
      include: [this._getStateOrderField()],
    };
  }
}

MeasurableModel.MEASURABLE_STATE = MEASURABLE_STATE;

module.exports = {
  MeasurableModel,
};
