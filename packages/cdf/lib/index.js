const { Cdf } = require('./lib/cdf');
const { Pdf } = require('./lib/pdf');
const { Samples } = require('./lib/samples');
const {
  ContinuousDistributionCombination,
} = require('./lib/continuousDistributionCombination');
const { ContinuousDistribution } = require('./lib/continuousDistribution');
const { scoringFunctions } = require('./lib/scoring');

module.exports = {
  Pdf,
  Cdf,
  Samples,
  ContinuousDistribution,
  ContinuousDistributionCombination,
  scoringFunctions
};
