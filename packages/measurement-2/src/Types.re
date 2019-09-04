open Belt.Result;

type unresolvableTypes = [
  | `AMBIGUOUS
  | `FALSE_CONDITIONAL
  | `OTHER
  | `RESULT_NOT_AVAILABLE
];

type commentTypes = [ | `GENERIC | `QUESTION_FEEDBACK | `UPDATE];

module Cdf = {
  type t = {
    xs: array(float),
    ys: array(float),
  };

  let verifyHasLength = ({xs, ys}) =>
    Array.length(xs) != Array.length(ys)
      ? Error("Arrays must have more than 0 elements.") : Ok({xs, ys});

  let verifySameLength = ({xs, ys}) =>
    Array.length(xs) == 0 || Array.length(ys) == 0
      ? Error("Array length must be the same.") : Ok({xs, ys});

  // todo: verify is increasing
  let make = ({xs, ys}) =>
    {xs, ys} |> verifyHasLength |> Rationale.Result.bind(_, verifySameLength);
};

module Percentage = {
  type t = float;

  let inverse = (t: t): t => 1. -. t;

  let verifyInRange = (f: float) =>
    if (f < 0. || f > 1.) {
      Error("Percentage must be between 0 and 1.");
    } else {
      Ok(f);
    };

  let make = (t: t) => verifyInRange(t);
};

module Measurement = {
  type t = [
    | `Cdf(Cdf.t)
    | `Float(float)
    | `Percentage(Percentage.t)
    | `UnresolvableResolution(unresolvableTypes)
    | `Comment(commentTypes)
  ];
};

module ValidScoringCombination = {
  type marketCdfCdf = {
    prediction: Cdf.t,
    market: Cdf.t,
    resolution: Cdf.t,
    sampleCount: int,
  };
  type marketCdfFloat = {
    prediction: Cdf.t,
    market: Cdf.t,
    resolution: float,
  };
  type marketPercentagePercentage = {
    prediction: Percentage.t,
    market: Percentage.t,
    resolution: Percentage.t,
  };
  type nonMarketCdfCdf = {
    prediction: Cdf.t,
    resolution: Cdf.t,
    sampleCount: int,
  };
  type nonMarketCdfFloat = {
    prediction: Cdf.t,
    resolution: float,
  };
  type nonMarketPercentagePercentage = {
    prediction: Percentage.t,
    resolution: Percentage.t,
  };
  type t = [
    | `MarketCdfCdf(marketCdfCdf)
    | `MarketCdfFloat(marketCdfFloat)
    | `MarketPercentagePercentage(marketPercentagePercentage)
    | `nonMarketCdfCdf(nonMarketCdfCdf)
    | `nonMarketCdfFloat(nonMarketCdfFloat)
    | `nonMarketPercentagePercentage(nonMarketPercentagePercentage)
  ];
};

module ScoringCombination = {
  // type combinationType =
  //   | MarketCdfCdf
  //   | MarketCdfFloat
  //   | MarketPercentagePercentage
  //   | NonMarketCdfCdf
  //   | NonMarketCdfFloat
  //   | NonMarketPercentagePercentage;

  type t = {
    prediction: Measurement.t,
    market: option(Measurement.t),
    resolution: Measurement.t,
    sampleCount: option(int),
  };

  let toValidScoringCombination =
      ({prediction, resolution, market, sampleCount}: t)
      : option(ValidScoringCombination.t) => {
    switch (prediction, market, resolution, sampleCount) {
    | (
        `Cdf(prediction),
        Some(`Cdf(market)),
        `Cdf(resolution),
        Some(sampleCount),
      ) =>
      Some(`MarketCdfCdf({prediction, market, resolution, sampleCount}))
    | (`Cdf(prediction), Some(`Cdf(market)), `Float(resolution), _) =>
      Some(`MarketCdfFloat({prediction, market, resolution}))
    | (
        `Percentage(prediction),
        Some(`Percentage(market)),
        `Percentage(resolution),
        _,
      ) =>
      Some(`MarketPercentagePercentage({prediction, market, resolution}))
    | (`Cdf(prediction), None, `Cdf(resolution), Some(sampleCount)) =>
      Some(`nonMarketCdfCdf({prediction, resolution, sampleCount}))
    | (`Cdf(prediction), None, `Float(resolution), _) =>
      Some(`nonMarketCdfFloat({prediction, resolution}))
    | (`Percentage(prediction), None, `Percentage(resolution), _) =>
      Some(`nonMarketPercentagePercentage({prediction, resolution}))
    | _ => None
    };
  };
};