open Belt.Result;

module UnresolvableResolution = {
  type t = [
    | `AMBIGUOUS
    | `FALSE_CONDITIONAL
    | `OTHER
    | `RESULT_NOT_AVAILABLE
  ];

  let toString = (e: t): string =>
    switch (e) {
    | `AMBIGUOUS => "AMBIGUOUS"
    | `FALSE_CONDITIONAL => "FALSE_CONDITIONAL"
    | `OTHER => "OTHER"
    | `RESULT_NOT_AVAILABLE => "RESULT_NOT_AVAILABLE"
    };
};

module Comment = {
  type t = [ | `GENERIC | `QUESTION_FEEDBACK | `UPDATE];

  let fromString = e =>
    switch (e) {
    | "GENERIC" => `GENERIC
    | "QUESTION_FEEDBACK" => `QUESTION_FEEDBACK
    | "UPDATE" => `UPDATE
    | _ => Js.Exn.raiseError("Invalid GraphQL Comment: " ++ e)
    };

  let toPublicString = (e: t): string =>
    switch (e) {
    | `GENERIC => "Generic"
    | `QUESTION_FEEDBACK => "Question Feedback"
    | `UPDATE => "Update"
    };
};

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
  type typeNames =
    | Cdf
    | Float
    | Binary
    | Percentage
    | UnresolvableResolution
    | Comment;

  type t = [
    | `Cdf(Cdf.t)
    | `Float(float)
    | `Binary(bool)
    | `Percentage(Percentage.t)
    | `UnresolvableResolution(UnresolvableResolution.t)
    | `Comment(Comment.t)
  ];

  let isCdf = (t: t) =>
    switch (t) {
    | `Cdf(_) => true
    | _ => false
    };

  let toCdf = (t: t) =>
    switch (t) {
    | `Cdf(cdf) => Some(cdf)
    | _ => None
    };

  let toTypeName = (t: t) =>
    switch (t) {
    | `Cdf(_) => Cdf
    | `Float(_) => Cdf
    | `Binary(_) => Cdf
    | `Percentage(_) => Cdf
    | `UnresolvableResolution(_) => Cdf
    | `Comment(_) => Cdf
    };
};

type time = int;
module TypedMeasurementWithTime = {
  type t('a) = {
    measurement: 'a,
    time,
  };
  type ts('a) = array(t('a));
  let make = (time, measurement) => {measurement, time};
};

module MeasurementWithTime = {
  type t = {
    measurement: Measurement.t,
    time,
  };
  type ts = array(t);
  // let tsToTypedTs = ({measurement, time}: t) =>
  //   switch (measurement) {
  //   | `Cdf(v) => `Cdf(TypedMeasurementWithTime.make(3.0, time))
  //   };
};

let foo: MeasurementWithTime.ts = [|{measurement: `Float(3.0), time: 34}|];

>>>>>>> gentype-2
module ValidScoringCombination = {
  type marketCdfCdf = {
    agentPrediction: Cdf.t,
    marketPrediction: Cdf.t,
    resolution: Cdf.t,
    sampleCount: int,
  };
  type marketCdfFloat = {
    agentPrediction: Cdf.t,
    marketPrediction: Cdf.t,
    resolution: float,
  };
  type marketPercentagePercentage = {
    agentPrediction: Percentage.t,
    marketPrediction: Percentage.t,
    resolution: Percentage.t,
  };
  type nonMarketCdfCdf = {
    agentPrediction: Cdf.t,
    resolution: Cdf.t,
    sampleCount: int,
  };
  type nonMarketCdfFloat = {
    agentPrediction: Cdf.t,
    resolution: float,
  };
  type nonMarketPercentagePercentage = {
    agentPrediction: Percentage.t,
    resolution: Percentage.t,
  };
  type t = [
    | `MarketCdfCdf(marketCdfCdf)
    | `MarketCdfFloat(marketCdfFloat)
    | `MarketPercentagePercentage(marketPercentagePercentage)
    | `NonMarketCdfCdf(nonMarketCdfCdf)
    | `NonMarketCdfFloat(nonMarketCdfFloat)
    | `NonMarketPercentagePercentage(nonMarketPercentagePercentage)
  ];

  let make = (t: t) => t;
};

module ScoringCombination = {
  type marketScoreType =
    | MarketScore
    | NonMarketScore;

  type t = {
    agentPrediction: Measurement.t,
    marketPrediction: option(Measurement.t),
    resolution: Measurement.t,
    sampleCount: option(int),
    marketScoreType,
  };

  let toValidScoringCombination =
      (
        {
          agentPrediction,
          resolution,
          marketPrediction,
          sampleCount,
          marketScoreType,
        }: t,
      )
      : option(ValidScoringCombination.t) => {
    switch (
      marketScoreType,
      agentPrediction,
      marketPrediction,
      resolution,
      sampleCount,
    ) {
    | (
        MarketScore,
        `Cdf(agentPrediction),
        Some(`Cdf(marketPrediction)),
        `Cdf(resolution),
        Some(sampleCount),
      ) =>
      Some(
        `MarketCdfCdf({
          agentPrediction,
          marketPrediction,
          resolution,
          sampleCount,
        }),
      )
    | (
        MarketScore,
        `Cdf(agentPrediction),
        Some(`Cdf(marketPrediction)),
        `Float(resolution),
        _,
      ) =>
      Some(`MarketCdfFloat({agentPrediction, marketPrediction, resolution}))
    | (
        MarketScore,
        `Percentage(agentPrediction),
        Some(`Percentage(marketPrediction)),
        `Percentage(resolution),
        _,
      ) =>
      Some(
        `MarketPercentagePercentage({
          agentPrediction,
          marketPrediction,
          resolution,
        }),
      )
    | (
        NonMarketScore,
        `Cdf(agentPrediction),
        None,
        `Cdf(resolution),
        Some(sampleCount),
      ) =>
      Some(`NonMarketCdfCdf({agentPrediction, resolution, sampleCount}))
    | (NonMarketScore, `Cdf(agentPrediction), None, `Float(resolution), _) =>
      Some(`NonMarketCdfFloat({agentPrediction, resolution}))
    | (
        NonMarketScore,
        `Percentage(agentPrediction),
        None,
        `Percentage(resolution),
        _,
      ) =>
      Some(`NonMarketPercentagePercentage({agentPrediction, resolution}))
    | _ => None
    };
  };
};

module ValidScoringCombinationGroupOverTime = {
  type time = int;

  type measurementWithTime('a) = {
    measurement: 'a,
    time,
  };

  type measurementsWithTime('a) = array(measurementWithTime('a));

  type marketCdfCdf = {
    agentPredictions: measurementsWithTime(Cdf.t),
    marketPredictions: measurementsWithTime(Cdf.t),
    resolution: measurementWithTime(Cdf.t),
    sampleCount: int,
  };
  type marketCdfFloat = {
    agentPredictions: measurementsWithTime(Cdf.t),
    marketPredictions: measurementsWithTime(Cdf.t),
    resolutions: measurementWithTime(float),
  };
  type marketPercentagePercentage = {
    agentPredictions: measurementsWithTime(Percentage.t),
    marketPredictions: measurementsWithTime(Percentage.t),
    resolution: measurementWithTime(Percentage.t),
  };
  type nonMarketCdfCdf = {
    agentPredictions: measurementsWithTime(Cdf.t),
    resolution: Cdf.t,
    sampleCount: int,
  };
  type nonMarketCdfFloat = {
    agentPredictions: measurementsWithTime(Cdf.t),
    resolution: float,
  };
  type nonMarketPercentagePercentage = {
    agentPredictions: measurementsWithTime(Percentage.t),
    resolution: Percentage.t,
  };
  type measurementGroup = [
    | `MarketCdfCdf(marketCdfCdf)
    | `MarketCdfFloat(marketCdfFloat)
    | `MarketPercentagePercentage(marketPercentagePercentage)
    | `NonMarketCdfCdf(nonMarketCdfCdf)
    | `NonMarketCdfFloat(nonMarketCdfFloat)
    | `NonMarketPercentagePercentage(nonMarketPercentagePercentage)
  ];

  type t = {
    measurementGroup,
    beginningTime: time,
  };

  let make = (t: t) => t;
};

module ScoringCombinationGroupOverTime = {
  type time = int;

  type measurementWithTime('a) = {
    measurement: 'a,
    time,
  };

  type measurementsWithTime('a) = array(measurementWithTime('a));

  type genMarket('a, 'b) = {
    agentPredictions: measurementsWithTime('a),
    resolutions: measurementsWithTime('a),
    resolution: measurementsWithTime('b),
    beginningTime: time,
  };

  type genNonMarket('a, 'b) = {
    agentPredictions: TypedMeasurementWithTime.ts('a),
    resolution: TypedMeasurementWithTime.t('b),
    beginningTime: time,
  };

  type t = [
    | `CdfCdf(genMarket(Cdf.t, Cdf.t))
    | `CdfFloat(genMarket(Cdf.t, float))
    | `MarketPercentagePercentage(genMarket(Percentage.t, Percentage.t))
    | `NonMarketCdfCdf(genNonMarket(Cdf.t, Cdf.t))
    | `NonMarketCdfFloat(genNonMarket(Cdf.t, float))
    | `NonMarketPercentagePercentage(
        genNonMarket(Percentage.t, Percentage.t),
      )
  ];
};

module Interface = {
  type time = int;

  module MeasurementWithTime = {
    type t = {
      measurement: Measurement.t,
      time,
    };
    type ts = array(t);
  };

  module MarketCombination = {
    type t = {
      agentPredictions: MeasurementsWithTime.t,
      marketPredictions: MeasurementsWithTime.t,
      resolution: MeasurementWithTime.t,
      beginningTime: time,
    };
    // let toMain =
    //     ({agentPredictions, marketPredictions, resolution, beginningTime}: t) => {
    //       let agentPreds =
    //     };
  };

  module NonMarketCombination = {
    type t = {
      agentPredictions: measurementsWithTime,
      marketPredictions: measurementsWithTime,
      resolution: measurementWithTime,
      beginningTime: time,
    };
    let score =
        ({agentPredictions, marketPredictions, resolution, beginningTime}: t) => 3.;
  };
};