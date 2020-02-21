const _ = require('lodash');
const moment = require('moment');
const { MEASUREMENT_VALUE } = require('@foretold/measurement-value/enums');

const { MeasurablesData } = require('../../data');
const { MeasurementsData } = require('../../data');

const { MEASUREMENT_COMPETITOR_TYPE } = require('../../enums');
const { MEASURABLE_STATE } = require('../../enums');

const { Pagination } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { Params } = require('../../data/classes');
const logger = require('../../lib/log');

const log = logger.module('sync/actions/measurables-state-machine');

class MeasurablesStateMachine {
  constructor() {
    this.measurables = new MeasurablesData();
    this.measurements = new MeasurementsData();
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async toJudgementPending() {
    let transaction;
    try {
      transaction = await this.measurables.getTransaction();
    } catch (e) {
      log.trace('Transaction Error', e.message, e);
      throw e;
    }

    try {
      const measurables = await this._getMeasurablesToPending(transaction);

      for (const measurable of measurables) {
        try {
          const result = !!(await this._judgementPending(
            measurable,
            transaction,
          ));

          log.trace(
            `\x1b[35mMeasurable ID = "${_.get(measurable, 'id', null)}", `
            + `Result = "${result}".\x1b[0m`,
          );
        } catch (err) {
          log.error(err.message);
        }
      }

      await this.measurables.commit(transaction);
    } catch (e) {
      log.trace(e.message, e);
      await this.measurables.rollback(transaction);
    }

    return true;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async toResolving() {
    let transaction;
    try {
      transaction = await this.measurables.getTransaction();
    } catch (e) {
      log.trace('Transaction Error', e.message, e);
      throw e;
    }

    try {
      const measurables = await this._getMeasurablesToResolutionResponse(
        transaction,
      );

      for (const measurable of measurables) {
        try {
          const result = !!(await this._processResolution(
            measurable,
            transaction,
          ));

          log.trace(
            `\x1b[35mMeasurable ID = "${_.get(measurable, 'id', null)}", `
            + `Result = "${result}".\x1b[0m`,
          );
        } catch (err) {
          log.error(err.message);
        }
      }

      await this.measurables.commit(transaction);
    } catch (e) {
      log.trace(e.message, e);
      await this.measurables.rollback(transaction);
    }

    return true;
  }

  /**
   * @param {Definitions.Measurement} measurement
   * @returns {Promise<boolean>}
   */
  async measurableStateTransition(measurement) {
    const { competitorType } = measurement;
    const measurable = await measurement.getMeasurable();

    if (competitorType === MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE) {
      await this._judged(measurable);
      return true;
    }

    if (competitorType === MEASUREMENT_COMPETITOR_TYPE.UNRESOLVED) {
      await this._closedAsUnresolved(measurable);
      return true;
    }

    return false;
  }

  /**
   * @param {object} transaction
   * @return {Promise<Models.Measurable[]>}
   * @protected
   */
  async _getMeasurablesToPending(transaction) {
    const filter = new Filter({ needsToBePending: true });
    const pagination = new Pagination({ limit: 100 });
    const options = new Options({
      transaction, lock: true, skipLocked: true, isAdmin: true,
    });
    return this.measurables.getAll(filter, pagination, options);
  }

  /**
   * @param {object} transaction
   * @return {Promise<Models.Measurable[]>}
   * @protected
   */
  async _getMeasurablesToResolutionResponse(transaction) {
    const filter = new Filter({ needsResolutionResponse: true });
    const pagination = new Pagination({ limit: 100 });
    const options = new Options({
      transaction, lock: true, skipLocked: true, isAdmin: true,
    });
    return this.measurables.getAll(filter, pagination, options);
  }

  /**
   * @param {Definitions.Measurable} measurable
   * @param {object} transaction
   * @return {Promise<boolean>}
   * @protected
   */
  async _processResolution(measurable, transaction) {
    const asFloat = await this.measurables.resolutionEndpointResponse(
      measurable,
    );

    if (asFloat != null) {
      const measurement = new Data({
        agentId: measurable.creatorId,
        competitorType: MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
        measurableId: measurable.id,
        value: {
          [MEASUREMENT_VALUE.floatPoint]: asFloat,
        },
      });
      const options = new Options({ transaction });
      await this.measurements.createOne(measurement, options);
    }

    // Judge even there is no correct response.
    await this._judged(measurable, transaction);

    return asFloat != null;
  }

  /**
   * @param {Definitions.Measurable} measurable
   * @param {object | null} transaction
   * @return {Promise<Models.NotificationStatus>}
   * @protected
   */
  async _judged(measurable, transaction = null) {
    const state = MEASURABLE_STATE.JUDGED;
    return this._changeState(measurable, state, transaction);
  }

  /**
   * @param {Definitions.Measurable} measurable
   * @param {object | null} transaction
   * @return {Promise<Models.NotificationStatus>}
   * @protected
   */
  async _closedAsUnresolved(measurable, transaction = null) {
    const state = MEASURABLE_STATE.CLOSED_AS_UNRESOLVED;
    return this._changeState(measurable, state, transaction);
  }

  /**
   * @param {Definitions.Measurable} measurable
   * @param {object | null} transaction
   * @return {Promise<Models.NotificationStatus>}
   * @protected
   */
  async _judgementPending(measurable, transaction = null) {
    const state = MEASURABLE_STATE.JUDGEMENT_PENDING;
    return this._changeState(measurable, state, transaction);
  }

  /**
   * @param {Definitions.Measurable} measurable
   * @param {string} state
   * @param {object | null} transaction
   * @return {Promise<Models.NotificationStatus>}
   * @protected
   */
  async _changeState(measurable, state, transaction = null) {
    const stateUpdatedAt = moment.utc().toDate();
    const params = new Params({ id: measurable.id });
    const data = new Data({ state, stateUpdatedAt });
    const options = new Options({ transaction });
    return this.measurables.updateOne(params, data, options);
  }
}

module.exports = {
  MeasurablesStateMachine,
};
