// module WithMarket = {
//   type combination('a, 'b) = {
//     agentPrediction: 'a,
//     marketPrediction: 'a,
//     resolution: 'b,
//   };
//   type cdfCdf = combination(Cdf.t, Cdf.t);
//   type cdfFloat = combination(Cdf.t, float);
//   type percentagePercentage = combination(Percentage.t, Percentage.t);
//   type t = [
//     | `CdfCdf(cdfCdf)
//     | `CdfFloat(cdfFloat)
//     | `PercentagePercentage(percentagePercentage)
//   ];
// };

module T = {
  type combination('a, 'b) = {
    agentPrediction: 'a,
    marketPrediction: option('a),
    resolution: 'b,
  };
  type cdfCdf = combination(Cdf.t, Cdf.t);
  type cdfFloat = combination(Cdf.t, float);
  type percentagePercentage = combination(Percentage.t, Percentage.t);
  type t = [
    | `CdfCdf(cdfCdf)
    | `CdfFloat(cdfFloat)
    | `PercentagePercentage(percentagePercentage)
  ];
  let marketPredictionExists = (m: t) =>
    switch (m) {
    | `CdfCdf({marketPrediction: Some(_)}) => true
    | `CdfFloat({marketPrediction: Some(_)}) => true
    | `PercentagePercentage({marketPrediction: Some(_)}) => true
    | `CdfCdf({marketPrediction: None}) => false
    | `CdfFloat({marketPrediction: None}) => false
    | `PercentagePercentage({marketPrediction: None}) => false
    };
  // There must be some more elegant way to do this, but I'm really not sure what it is.
  // let toWithMarket = (t: t): Belt.Result.t(WithMarket.t, string) =>
  //   switch (t) {
  //   | `CdfCdf({
  //       agentPrediction,
  //       marketPrediction: Some(marketPrediction),
  //       resolution,
  //     }) =>
  //     Belt.Result.Ok(
  //       `CdfCdf({agentPrediction, marketPrediction, resolution}),
  //     )
  //   | `CdfFloat({
  //       agentPrediction,
  //       marketPrediction: Some(marketPrediction),
  //       resolution,
  //     }) =>
  //     Belt.Result.Ok(
  //       `CdfFloat({agentPrediction, marketPrediction, resolution}),
  //     )
  //   | `PercentagePercentage({
  //       agentPrediction,
  //       marketPrediction: Some(marketPrediction),
  //       resolution,
  //     }) =>
  //     Belt.Result.Ok(
  //       `PercentagePercentage({
  //         agentPrediction,
  //         marketPrediction,
  //         resolution,
  //       }),
  //     )
  //   | _ => Error("Market Prediction Needed")
  //   };
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