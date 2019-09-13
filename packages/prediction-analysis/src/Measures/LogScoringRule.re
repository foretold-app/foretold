open Belt.Result;
let log2Error = Js.Math.log2;

let marketCdfCdf =
    (
      {agentPrediction, marketPrediction, resolution}:
        PredictionResolutionGroup.WithMarket.combination(Cdf.t, Cdf.t),
      sampleCount: int,
    ) =>
  Ok(3.0);

let nonMarketCdfCdf =
    (
      {agentPrediction, resolution}:
        PredictionResolutionGroup.combination(Cdf.t, Cdf.t),
      sampleCount: int,
    ) =>
  Ok(3.0);

let marketCdfFloat =
    (
      {agentPrediction, marketPrediction, resolution}:
        PredictionResolutionGroup.WithMarket.combination(Cdf.t, float),
    ) => {
  let pdfY = e => e |> CdfMeasures.toPdf |> Distribution.T.findY(resolution);
  let (agentPrediction, marketPrediction) = (
    pdfY(agentPrediction),
    pdfY(marketPrediction),
  );
  agentPrediction /. marketPrediction |> log2Error;
};

let nonMarketCdfFloat =
    (
      {agentPrediction, resolution}:
        PredictionResolutionGroup.combination(Cdf.t, float),
    ) =>
  agentPrediction
  |> CdfMeasures.toPdf
  |> Distribution.T.findY(resolution)
  |> log2Error;

let marketPercentagePercentage =
    (
      {agentPrediction, marketPrediction, resolution}:
        PredictionResolutionGroup.WithMarket.combination(
          Percentage.t,
          Percentage.t,
        ),
    ) =>
  //   TODO: Handle cases where market is 0 or 1
  Percentage.(
    {
      let positiveFactor = {
        let (marketPrediction, agentPrediction, resolution) = (
          toFloat(marketPrediction),
          toFloat(agentPrediction),
          toFloat(resolution),
        );
        resolution *. log2Error(agentPrediction /. marketPrediction);
      };
      let negativeFactor = {
        let (marketPrediction, agentPrediction, resolution) = (
          toFloat(inverse(marketPrediction)),
          toFloat(inverse(agentPrediction)),
          toFloat(inverse(resolution)),
        );
        resolution *. log2Error(agentPrediction /. marketPrediction);
      };
      positiveFactor +. negativeFactor;
    }
  );
let nonMarketPercentagePercentage =
    (
      {agentPrediction, resolution}:
        PredictionResolutionGroup.combination(Percentage.t, Percentage.t),
    ) =>
  Percentage.(
    {
      let positiveFactor = {
        let (agentPrediction, resolution) = (
          toFloat(agentPrediction),
          toFloat(resolution),
        );
        resolution *. log2Error(agentPrediction /. resolution);
      };
      let negativeFactor = {
        let (agentPrediction, resolution) = (
          toFloat(inverse(agentPrediction)),
          toFloat(inverse(resolution)),
        );
        resolution *. log2Error(agentPrediction /. resolution);
      };
      positiveFactor +. negativeFactor;
    }
  );

type group = [
  | `MarketScore(PredictionResolutionGroup.WithMarket.t)
  | `NonMarketScore(PredictionResolutionGroup.t)
];

let run = (~scoringCombination: group, ~sampleCount, ()) => {
  switch (scoringCombination) {
  | `MarketScore(`CdfCdf(v)) => marketCdfCdf(v, sampleCount)
  | `MarketScore(`CdfFloat(v)) => Ok(marketCdfFloat(v))
  | `MarketScore(`PercentagePercentage(v)) =>
    Ok(marketPercentagePercentage(v))
  | `NonMarketScore(`CdfCdf(v)) => nonMarketCdfCdf(v, sampleCount)
  | `NonMarketScore(`CdfFloat(v)) => Ok(nonMarketCdfFloat(v))
  | `NonMarketScore(`PercentagePercentage(v)) =>
    Ok(nonMarketPercentagePercentage(v))
  };
};