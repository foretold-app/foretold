const models = require('./definitions');
const { MEASURABLE_STATE } = require('../enums');

const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class MeasurableModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Measurable,
      sequelize: models.sequelize,
    });
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
   * @param {Layers.Filter} [filter]
   * @param {Defs.AgentID} [filter.userId]
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
   * @protected
   */
  _getOrder() {
    return [
      [this.sequelize.col('stateOrder'), 'ASC'],
      ['createdAt', 'DESC'],
    ];
  }

  /**
   * @param {Layers.Models.ModelOptions} _options
   * @return {{include: Sequelize.literal|*[]}}
   * @protected
   */
  _getAttributes(_options = {}) {
    return {
      include: [this._getStateOrderField()],
    };
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

    return this.literal(
      `(CASE WHEN "state"='${OPEN}' THEN 1 `
      + `WHEN "state"='${JUDGEMENT_PENDING}' THEN 2 `
      + `WHEN "state"='${JUDGED}' THEN 3 `
      + `WHEN "state"='${CLOSED_AS_UNRESOLVED}' THEN 3 `
      + 'ELSE 5 END) AS "stateOrder"',
    );
  }
}

MeasurableModel.MEASURABLE_STATE = MEASURABLE_STATE;

module.exports = {
  MeasurableModel,
};
