const _ = require('lodash');
const data = require('../../data');

class MeasurablesStates {
  constructor() {
    this.data = data;
  }

  main() {
    return Promise.resolve(true)
      .then(() => this.data.measurables.getAllJudgementPendingNext(this.reducerFn));
  }

  reducerFn(measurables) {
    console.log('data', data);
    return;
  }
}

module.exports = {
  MeasurablesStates,
};
