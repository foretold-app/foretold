open Belt.Result;
let log2Error = Js.Math.log2;

type predictionGroupErrorType = [
  | `MarketScore(PredictionResolutionGroup.WithMarket.t)
  | `NonMarketScore(PredictionResolutionGroup.t)
];

module PredictionGroupError = {
  let marketCdfCdf =
      (
        sampleCount: int,
        {agentPrediction, marketPrediction, resolution}:
          PredictionResolutionGroup.WithMarket.combination(Cdf.t, Cdf.t),
      ) =>
    CdfLibraryImporter.PredictionResolutionGroup.logScoreMarketCdfCdf(
      ~sampleCount,
      ~agentPrediction=agentPrediction |> Cdf.toDistribution,
      ~marketPrediction=marketPrediction |> Cdf.toDistribution,
      ~resolution=resolution |> Cdf.toDistribution,
    );

  let nonMarketCdfCdf =
      (
        sampleCount: int,
        {agentPrediction, resolution}:
          PredictionResolutionGroup.combination(Cdf.t, Cdf.t),
      ) =>
    CdfLibraryImporter.PredictionResolutionGroup.logScoreNonMarketCdfCdf(
      ~sampleCount,
      ~agentPrediction=agentPrediction |> Cdf.toDistribution,
      ~resolution=resolution |> Cdf.toDistribution,
    );

  let marketCdfFloat =
      (
        {agentPrediction, marketPrediction, resolution}:
          PredictionResolutionGroup.WithMarket.combination(Cdf.t, float),
      ) => {
    let pdfY = e => e |> Cdf.toPdf |> Distribution.T.findY(resolution);
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
    |> Cdf.toPdf
    |> Distribution.T.findY(resolution)
    |> log2Error;

  //   TODO: Handle cases where market is 0 or 1
  let marketPercentagePercentage =
      (
        {agentPrediction, marketPrediction, resolution}:
          PredictionResolutionGroup.WithMarket.combination(
            Percentage.t,
            Percentage.t,
          ),
      ) =>
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

  let run = (~scoringCombination: predictionGroupErrorType, ~sampleCount, ()) => {
    switch (scoringCombination) {
    | `MarketScore(`CdfCdf(v)) => Ok(marketCdfCdf(sampleCount, v))
    | `MarketScore(`CdfFloat(v)) => Ok(marketCdfFloat(v))
    | `MarketScore(`PercentagePercentage(v)) =>
      Ok(marketPercentagePercentage(v))
    | `NonMarketScore(`CdfCdf(v)) => Ok(nonMarketCdfCdf(sampleCount, v))
    | `NonMarketScore(`CdfFloat(v)) => Ok(nonMarketCdfFloat(v))
    | `NonMarketScore(`PercentagePercentage(v)) =>
      Ok(nonMarketPercentagePercentage(v))
    };
  };
};

module DifferentialEntropy = {
  let ofCdf = (sampleCount, cdf) =>
    cdf
    |> Cdf.toDistribution
    |> CdfLibraryImporter.Distribution.differentialEntropy(sampleCount);

  let ofPercentage = p =>
    Percentage.(
      {
        let positiveFactor = {
          let p = toFloat(p);
          p *. log2Error(p);
        };
        let negativeFactor = {
          let p = toFloat(inverse(p));
          p *. log2Error(p);
        };
        positiveFactor +. negativeFactor;
      }
    );

  let run = (sampleCount, m: MeasurementValue.t) => {
    switch (m) {
    | `Cdf(m) => Some(ofCdf(sampleCount, m))
    | `Percentage(p) => Some(ofPercentage(p))
    | _ => None
    };
  };
};

let predictionGroupError = PredictionGroupError.run;
let differentialEntropy = DifferentialEntropy.run;