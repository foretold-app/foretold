const data = require('../../data');

class MeasurablesStates {
  constructor() {
    this.data = data;
  }

  main() {
    return Promise.resolve(true)
      .then(() => this.data.measurables.getAllJudgementPendingNext());
  }
}

module.exports = {
  MeasurablesStates,
};
