const models = require('./definitions');
const { MEASURABLE_STATE } = require('../../enums');

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
   * Default order.
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
   * @param {Layers.Pagination} pagination
   * @returns {[*, string][]}
   * @protected
   */
  _getOrderFromPagination(pagination) {
    const order = pagination.getOrder();
    return order.map((orderItem) => {
      const switchedOrderName = this._switchField(orderItem.field);
      const switchedDirection = this._switchDirection(orderItem.direction);
      return [switchedOrderName, switchedDirection];
    });
  }

  /**
   * @param {string} name
   * @returns {object | string}
   * @protected
   */
  _switchField(name = '') {
    if (name === 'refreshedAt') {
      return this._refreshedAtLiteral();
    }
    if (name === 'stateOrder') {
      return this.sequelize.col('stateOrder');
    }
    return name;
  }

  /**
   * @param {string} name
   * @returns {object | string}
   * @protected
   */
  _switchDirection(name = '') {
    if (name === 'ASC') {
      return 'ASC NULLS LAST';
    }
    if (name === 'DESC') {
      return 'DESC NULLS LAST';
    }
    return name;
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
   * @return {Sequelize.literal}
   */
  _refreshedAtLiteral() {
    return this.literal(this._refreshedAt());
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

  /**
   * @protected
   * @return {string}
   */
  _refreshedAt() {
    return `(
        SELECT "updatedAt"
        FROM "Measurements"
        WHERE "Measurements"."measurableId" = "Measurable"."id"
        ORDER BY "Measurements"."updatedAt" DESC
        LIMIT 1
    )`;
  }
}

MeasurableModel.MEASURABLE_STATE = MEASURABLE_STATE;

module.exports = {
  MeasurableModel,
};
