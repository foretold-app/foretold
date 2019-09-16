const PredictionResolutionOverTime = jest.fn().mockImplementation(() => ({
  averagePointScore: () => 1,
  data: 2,
}));

const marketScore = 3;

module.exports = {
  marketScore,
  PredictionResolutionOverTime,
};
