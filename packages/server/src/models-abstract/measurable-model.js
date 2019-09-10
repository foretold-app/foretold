const _ = require('lodash');
const {
  MEASUREMENT_VALUE,
} = require('@foretold/measurement-value/enums/measurement-value');


const models = require('../models');
const { MEASURABLE_STATE } = require('../enums/measurable-state');

const { ModelPostgres } = require('./model-postgres');
const {
  MEASUREMENT_COMPETITOR_TYPE,
} = require('../enums/measurement-competitor-type');

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
      `(CASE WHEN "state"='${ OPEN }' THEN 1 ` +
      `WHEN "state"='${ JUDGEMENT_PENDING }' THEN 2 ` +
      `WHEN "state"='${ JUDGED }' THEN 3 ` +
      `WHEN "state"='${ CLOSED_AS_UNRESOLVED }' THEN 3 ` +
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
   * @todo: implement client for this code
   * @todo: do not remove
   * @param {Models.Measurable} measurable
   * @return {Promise<Models.Measurable>}
   */
  async processResolution(measurable) {
    const asFloat = await measurable.resolutionEndpointResponse;
    if (asFloat) {
      await this.sequelize.models.Measurement.create({
        agentId: measurable.creatorId,
        competitorType: MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
        measurableId: measurable.id,
        value: {
          dataType: MEASUREMENT_VALUE.floatPoint,
          data: asFloat,
        },
      });
    }
    // @todo: Do we really need it if we have hooks?
    await measurable.judged();
    return measurable;
  };

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
