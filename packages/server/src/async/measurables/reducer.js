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
}

module.exports = {
  Reducer,
};
