const _ = require('lodash');
const data = require('../../data');

class MeasurablesStates {
  constructor() {
    this.data = data;
  }

  main() {
    return Promise.resolve()
      .then(() => this.data.measurables.setJudgementPending())
      .then(() => true);
  }

}

module.exports = {
  MeasurablesStates,
};
