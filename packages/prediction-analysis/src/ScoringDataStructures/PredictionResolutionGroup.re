module WithMarket = {
  type combination('a, 'b) = {
    agentPrediction: 'a,
    marketPrediction: 'a,
    resolution: 'b,
  };
  type t = [
    | `CdfCdf(combination(Cdf.t, Cdf.t))
    | `CdfFloat(combination(Cdf.t, float))
    | `PercentagePercentage(combination(Percentage.t, Percentage.t))
  ];
};

module T = {
  type combination('a, 'b) = {
    agentPrediction: 'a,
    marketPrediction: option('a),
    resolution: 'b,
  };

  type t = [
    | `CdfCdf(combination(Cdf.t, Cdf.t))
    | `CdfFloat(combination(Cdf.t, float))
    | `PercentagePercentage(combination(Percentage.t, Percentage.t))
  ];

  let map = ((f1, f2, f3), t) =>
    switch (t) {
    | `CdfCdf(e) => `CdfCdf(f1(e))
    | `CdfFloat(e) => `CdfFloat(f2(e))
    | `PercentagePercentage(e) => `PercentagePercentage(f3(e))
    };

  // I think that "sequence" could be a better name, but am not sure.
  let transposeOption = t =>
    switch (t) {
    | `CdfCdf(Some(e)) => Some(`CdfCdf(e))
    | `CdfFloat(Some(e)) => Some(`CdfFloat(e))
    | `PercentagePercentage(Some(e)) => Some(`PercentagePercentage(e))
    | _ => None
    };

  let combinationToMarket =
      ({agentPrediction, marketPrediction, resolution}: combination('a, 'b))
      : option(WithMarket.combination('a, 'b)) =>
    switch (marketPrediction) {
    | Some(marketPrediction) =>
      Some({agentPrediction, marketPrediction, resolution})
    | None => None
    };

  let toWithMarket = (t: t) =>
    t
    |> map((combinationToMarket, combinationToMarket, combinationToMarket))
    |> transposeOption;
};

module TBuilder = {
  let make =
      (
        ~agentPrediction: MeasurementValue.t,
        ~marketPrediction: option(MeasurementValue.t)=None,
        ~resolution: MeasurementValue.t,
        (),
      )
      : Belt.Result.t(T.t, string) => {
    let measurements: Belt.Result.t(T.t, string) =
      switch (agentPrediction, marketPrediction, resolution) {
      | (
          `Cdf(agentPrediction),
          Some(`Cdf(marketPrediction)),
          `Cdf(resolution),
        ) =>
        Ok(
          `CdfCdf({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (
          `Cdf(agentPrediction),
          Some(`Cdf(marketPrediction)),
          `Float(resolution),
        ) =>
        Ok(
          `CdfFloat({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (
          `Percentage(agentPrediction),
          Some(`Percentage(marketPrediction)),
          `Percentage(resolution),
        ) =>
        Ok(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution,
          }),
        )
      | (
          `Percentage(agentPrediction),
          Some(`Percentage(marketPrediction)),
          `Binary(resolution),
        ) =>
        Ok(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: Some(marketPrediction),
            resolution: Percentage.fromBool(resolution),
          }),
        )
      | (`Cdf(agentPrediction), None, `Cdf(resolution)) =>
        Ok(`CdfCdf({agentPrediction, marketPrediction: None, resolution}))
      | (`Cdf(agentPrediction), None, `Float(resolution)) =>
        Ok(`CdfFloat({agentPrediction, marketPrediction: None, resolution}))
      | (`Percentage(agentPrediction), None, `Percentage(resolution)) =>
        Ok(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: None,
            resolution,
          }),
        )
      | (`Percentage(agentPrediction), None, `Binary(resolution)) =>
        Ok(
          `PercentagePercentage({
            agentPrediction,
            marketPrediction: None,
            resolution: Percentage.fromBool(resolution),
          }),
        )
      | _ =>
        Error(
          "#toValidScoringCombination error: Scoring Combination Not Valid.",
        )
      };
    measurements;
  };
};

include T;
let fromArbitraryMeasurementValues = TBuilder.make;