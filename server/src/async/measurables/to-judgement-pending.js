const _ = require('lodash');

const data = require('../../data');

class ToJudgementPending {
  constructor() {
    this.data = data;
  }

  async main() {
    const measurables = await this.data.measurables.needsToBePending();
    _.each(measurables, (measurable) => {
      measurable.judgementPending();
    });
    return true;
  }
}

module.exports = {
  ToJudgementPending,
};
