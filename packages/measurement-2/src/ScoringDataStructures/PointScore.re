type marketScoreType =
  | MarketScore
  | NonMarketScore;

let run = (scoringCombination: ScoringCombination.ValidScoringCombination.t) =>
  LogScoringRule.run(scoringCombination);