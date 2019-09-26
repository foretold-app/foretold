const _ = require('lodash');

const data = require('../../data');

class MeasurablesStateMachine {
  constructor() {
    this.data = data;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async toJudgementPending() {
    const measurables = await this.data.measurables.needsToBePending();

    _.each(measurables, (measurable) => {
      measurable.judgementPending();
    });

    return true;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async toResolving() {
    const measurables = await this.data.measurables.needsResolutionResponse();

    _.each(measurables, (measurable) => {
      this.data.measurables.processResolution(measurable);
    });

    return true;
  }
}

module.exports = {
  MeasurablesStateMachine,
};
