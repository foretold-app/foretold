const _ = require('lodash');
const assert = require('assert');

class BrierScore {

  constructor(probabilities, result) {
    assert(_.isBoolean(result), 'Result should be a boolean.');
    assert(_.isArray(probabilities), 'Probabilities should be an array.');
    assert(_.every(probabilities, _.isNumber), 'Probabilities should contain numbers only.');
    assert(_.max(probabilities) <= 100, 'Percentage Max is over than 100.');
    assert(_.min(probabilities) >= 0, 'Percentage Min is less than 0.');

    this.probabilities = probabilities;
    this.result = result;
  }

  /**
   * @return {number}
   */
  mean() {
    const brierScores = this.brierScores();
    return _.mean(brierScores);
  }

  /**
   * @return {number[]}
   */
  brierScores() {
    const brierScores = [];
    _.each(this.probabilities, (probability) => {
      const brierScore = this._brierScore(probability, this.result);
      brierScore.push(brierScore);
    });
    return brierScores;
  }

  /**
   * @private
   * @param {number} probability
   * @param {boolean} result
   * @return {number}
   */
  _brierScore(probability, result) {
    if (probability === BrierScore.PERCENTAGES_50) {
      return BrierScore.REGARDLESS_SCORE;
    }
    if (result === true) {
      if (probability === BrierScore.PERCENTAGES_100) {
        return BrierScore.BEST_SCORE;
      }
      return ((probability / 100) - 1) ** 2;
    }
    if (result === false) {
      if (probability === BrierScore.PERCENTAGES_100) {
        return BrierScore.WORST_SCORE;
      }
      return ((probability / 100) - 0) ** 2;
    }
    return BrierScore.WORST_SCORE;
  }
}

BrierScore.BEST_SCORE = 0;
BrierScore.WORST_SCORE = 1;
BrierScore.REGARDLESS_SCORE = 0.25;
BrierScore.PERCENTAGES_100 = 100;
BrierScore.PERCENTAGES_50 = 50;

module.exports = {
  BrierScore,
};
