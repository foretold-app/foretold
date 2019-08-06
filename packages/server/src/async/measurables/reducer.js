const _ = require('lodash');

const data = require('../../data');

class Reducer {
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
   * @todo: implementation
   * @public
   * @param {Models.Agent.id} agentId
   * @return {Promise<boolean>}
   */
  async resolving(agentId) {
    const measurables = await this.data.measurables.needsResolutionResponse();

    _.each(measurables, (measurable) => {
      measurable.processResolution(agentId);
    });

    return true;
  }
}

module.exports = {
  Reducer,
};
