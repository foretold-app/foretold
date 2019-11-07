open Belt.Result;
let log2Error = Js.Math.log2;

type predictionGroupErrorType = [
  | `MarketScore(PredictionResolutionGroup.WithMarket.t)
  | `NonMarketScore(PredictionResolutionGroup.t)
];

let floatSubtractionWithLimitation = (a, b, limiter) => {
  switch (a, b) {
  | (a, b) when a == infinity && b == infinity => nan
  | (a, b) when a == neg_infinity && b == neg_infinity => nan
  | (a, b) when a == nan || b == nan => nan
  | (a, b) => limiter(a) -. limiter(b)
  };
};
let resolutionUniformAdditionWeight =
  Js.Math.pow_float(~base=10.0, ~exp=-11.);

module PredictionGroupError = {
  let marketCdfCdf =
      (
        sampleCount: int,
        {agentPrediction, marketPrediction, resolution}:
          PredictionResolutionGroup.WithMarket.combination(Cdf.t, Cdf.t),
        scoreLimiter: float => float,
      ) => {
    let agentPrediction = agentPrediction |> Cdf.toDistribution;
    let marketPrediction = marketPrediction |> Cdf.toDistribution;
    let resolution = resolution |> Cdf.toDistribution;
    let agent =
      CdfLibraryImporter.PredictionResolutionGroup.logScoreNonMarketCdfCdf(
        ~sampleCount,
        ~agentPrediction,
        ~resolution,
        ~resolutionUniformAdditionWeight,
      );
    let market =
      CdfLibraryImporter.PredictionResolutionGroup.logScoreNonMarketCdfCdf(
        ~sampleCount,
        ~agentPrediction=marketPrediction,
        ~resolution,
        ~resolutionUniformAdditionWeight,
      );
    floatSubtractionWithLimitation(agent, market, scoreLimiter);
  };

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
      ~resolutionUniformAdditionWeight,
    );

  let marketCdfFloat =
      (
        {agentPrediction, marketPrediction, resolution}:
          PredictionResolutionGroup.WithMarket.combination(Cdf.t, float),
        scoreLimiter: float => float,
      ) => {
    let pdfY = e => e |> Cdf.toPdf |> Distribution.T.findY(resolution);
    let (agentPrediction, marketPrediction) = (
      pdfY(agentPrediction),
      pdfY(marketPrediction),
    );
    floatSubtractionWithLimitation(
      log2Error(agentPrediction),
      log2Error(marketPrediction),
      scoreLimiter,
    );
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
        switch (Percentage.toFloat(resolution)) {
        | 0.0 => negativeFactor
        | 1.0 => positiveFactor
        | _ => positiveFactor +. negativeFactor
        };
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
        switch (Percentage.toFloat(resolution)) {
        | 0.0 => negativeFactor
        | 1.0 => positiveFactor
        | _ => positiveFactor +. negativeFactor
        };
      }
    );

  let run =
      (
        ~scoringCombination: predictionGroupErrorType,
        ~sampleCount,
        ~scoreLimiter,
        (),
      ) => {
    switch (scoringCombination) {
    | `MarketScore(`CdfCdf(v)) =>
      Ok(marketCdfCdf(sampleCount, v, scoreLimiter))
    | `MarketScore(`CdfFloat(v)) => Ok(marketCdfFloat(v, scoreLimiter))
    | `MarketScore(`PercentagePercentage(v)) =>
      Ok(marketPercentagePercentage(v) |> scoreLimiter)
    | `NonMarketScore(`CdfCdf(v)) =>
      Ok(nonMarketCdfCdf(sampleCount, v) |> scoreLimiter)
    | `NonMarketScore(`CdfFloat(v)) =>
      Ok(nonMarketCdfFloat(v) |> scoreLimiter)
    | `NonMarketScore(`PercentagePercentage(v)) =>
      Ok(nonMarketPercentagePercentage(v) |> scoreLimiter)
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