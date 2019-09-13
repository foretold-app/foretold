open Belt.Result;
let log2Error = Js.Math.log2;

let marketCdfCdf =
    (
      {agentPrediction, marketPrediction, resolution}:
        PredictionResolutionGroup.WithMarket.combination(Cdf.t, Cdf.t),
      sampleCount: int,
    ) =>
  Ok(3.0);

let marketCdfFloat =
    (
      {agentPrediction, marketPrediction, resolution}:
        PredictionResolutionGroup.WithMarket.combination(Cdf.t, float),
    ) =>
  Ok(3.0);

let marketPercentagePercentage =
    (
      {agentPrediction, marketPrediction, resolution}:
        PredictionResolutionGroup.WithMarket.combination(
          Percentage.t,
          Percentage.t,
        ),
    ) =>
  Ok(
    //   TODO: Handle cases where market is 0 or 1
    Percentage.(
      {
        let positive =
          toFloat(resolution)
          *. log2Error(
               toFloat(agentPrediction) /. toFloat(marketPrediction),
             );
        let negative =
          toFloat(inverse(resolution))
          *. log2Error(
               toFloat(inverse(agentPrediction))
               /. toFloat(inverse(marketPrediction)),
             );
        positive +. negative;
      }
    ),
  );

let nonMarketCdfCdf =
    (
      {agentPrediction, resolution}:
        PredictionResolutionGroup.combination(Cdf.t, Cdf.t),
      sampleCount: int,
    ) =>
  Ok(3.0);

let nonMarketCdfFloat =
    (
      {agentPrediction, resolution}:
        PredictionResolutionGroup.combination(Cdf.t, float),
    ) =>
  Ok(3.0);

let nonMarketPercentagePercentage =
    (
      {agentPrediction, resolution}:
        PredictionResolutionGroup.combination(Percentage.t, Percentage.t),
    ) =>
  Ok(
    Percentage.(
      {
        let positive =
          toFloat(resolution)
          *. log2Error(toFloat(agentPrediction) /. toFloat(resolution));
        let negative =
          toFloat(inverse(resolution))
          *. log2Error(
               toFloat(inverse(agentPrediction))
               /. toFloat(inverse(resolution)),
             );
        positive +. negative;
      }
    ),
  );

type group = [
  | `MarketScore(PredictionResolutionGroup.WithMarket.t)
  | `NonMarketScore(PredictionResolutionGroup.t)
];

let run = (~scoringCombination: group, ~sampleCount, ()) => {
  switch (scoringCombination) {
  | `MarketScore(`CdfCdf(v)) => marketCdfCdf(v, sampleCount)
  | `MarketScore(`CdfFloat(v)) => marketCdfFloat(v)
  | `MarketScore(`PercentagePercentage(v)) => marketPercentagePercentage(v)
  | `NonMarketScore(`CdfCdf(v)) => nonMarketCdfCdf(v, sampleCount)
  | `NonMarketScore(`CdfFloat(v)) => nonMarketCdfFloat(v)
  | `NonMarketScore(`PercentagePercentage(v)) =>
    nonMarketPercentagePercentage(v)
  };
};