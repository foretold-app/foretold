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
    return this.probabilities.map((probability) => {
      return this._brierScore(probability, this.result);
    });
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
    if (probability === BrierScore.PERCENTAGES_100) {
      return result === true
        ? BrierScore.BEST_SCORE
        : BrierScore.WORST_SCORE;
    }

    const score = result === true
      ? ((probability / 100) - 1) ** 2
      : (probability / 100) ** 2;

    return _.round(score, 2);
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
