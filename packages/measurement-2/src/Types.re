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

  let isX = (toXFn, t: t) => t |> toXFn |> Rationale.Option.isSome;

  let toCdf = (t: t) =>
    switch (t) {
    | `Cdf(a) => Some(a)
    | _ => None
    };

  let toFloat = (t: t) =>
    switch (t) {
    | `Float(a) => Some(a)
    | _ => None
    };

  let toBinary = (t: t) =>
    switch (t) {
    | `Binary(a) => Some(a)
    | _ => None
    };

  let toPercentage = (t: t) =>
    switch (t) {
    | `Percentage(a) => Some(a)
    | _ => None
    };

  let toUnresolvableResolution = (t: t) =>
    switch (t) {
    | `UnresolvableResolution(a) => Some(a)
    | _ => None
    };

  let toComment = (t: t) =>
    switch (t) {
    | `Comment(a) => Some(a)
    | _ => None
    };

  let isCdf = isX(toCdf);
  let isFloat = isX(toFloat);
  let isBinary = isX(toBinary);
  let isPercentage = isX(toPercentage);
  let isComment = isX(toComment);

  let toTypeName = (t: t) =>
    switch (t) {
    | `Cdf(_) => Cdf
    | `Float(_) => Float
    | `Binary(_) => Binary
    | `Percentage(_) => Percentage
    | `UnresolvableResolution(_) => UnresolvableResolution
    | `Comment(_) => Comment
    };
};

type time = int;
module TypedMeasurementWithTime = {
  type t('a) = {
    measurement: 'a,
    time,
  };
  type tss('a) = array(t('a));
  let make = (time, measurement) => {measurement, time};
  type ts = [
    | `Cdf(tss(Cdf.t))
    | `Float(tss(float))
    | `Binary(tss(bool))
    | `Percentage(tss(Percentage.t))
    | `UnresolvableResolution(tss(UnresolvableResolution.t))
    | `Comment(tss(Comment.t))
  ];
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

let concatSome = (optionals: array(option('a))): array('a) =>
  optionals
  |> Js.Array.filter(Rationale.Option.isSome)
  |> Js.Array.map(
       Rationale.Option.toExn("Warning: This should not have happened"),
     );
let defaultEmpty = (o: option(array('a))): array('a) =>
  switch (o) {
  | Some(o) => o
  | None => [||]
  };

// let foo: MeasurementWithTime.ts = [|{measurement: `Float(3.0), time: 34}|];
let foo = [|`Float(3.0)|];
let typee =
  Belt.Array.get(foo, 0) |> Rationale.Option.map(Measurement.toTypeName);

let mainType: option(TypedMeasurementWithTime.ts) =
  switch (typee) {
  | Some(Cdf) =>
    Some(
      foo
      |> Belt.Array.map(_, Measurement.toCdf)
      |> concatSome
      |> Belt.Array.map(_, r =>
           TypedMeasurementWithTime.{time: 32, measurement: r}
         )
      |> (r => `Cdf(r)),
    )
  | _ => None
  };

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
} /* }*/ /*     }*/ /*     //     ({agentPredictions, marketPredictions, resolution, beginningTime}: t) => */ /*     //     }*/ /*   module NonMarketCombination = */ /*       agentPredictions: measurementsWithTime*/ /*       resolution: measurementWithTime*/ /*     }*/ /*         ({agentPredictions, marketPredictions, resolution, beginningTime}: t) => 3.*/ /*   }*/ /*     let score */ /*       beginningTime: time*/ /*       marketPredictions: measurementsWithTime*/ /*     type t = */ /*   }*/ /*     //       let agentPreds */ /*     // let toMain */;

// module ScoringCombinationGroupOverTime = {
//   type time = int;

//   type measurementWithTime('a) = {
//     measurement: 'a,
//     time,
//   };

//   type measurementsWithTime('a) = array(measurementWithTime('a));

//   type genMarket('a, 'b) = {
//     agentPredictions: measurementsWithTime('a),
//     resolutions: measurementsWithTime('a),
//     resolution: measurementsWithTime('b),
//     beginningTime: time,
//   };

//   type genNonMarket('a, 'b) = {
//     agentPredictions: TypedMeasurementWithTime.ts('a),
//     resolution: TypedMeasurementWithTime.t('b),
//     beginningTime: time,
//   };

//   type t = [
//     | `CdfCdf(genMarket(Cdf.t, Cdf.t))
//     | `CdfFloat(genMarket(Cdf.t, float))
//     | `MarketPercentagePercentage(genMarket(Percentage.t, Percentage.t))
//     | `NonMarketCdfCdf(genNonMarket(Cdf.t, Cdf.t))
//     | `NonMarketCdfFloat(genNonMarket(Cdf.t, float))
//     | `NonMarketPercentagePercentage(
//         genNonMarket(Percentage.t, Percentage.t),
//       )
//   ];
// };

// module Interface = {
//   type time = int;

//   module MeasurementWithTime = {
//     type t = {
//       measurement: Measurement.t,
//       time,
//     };
//     type ts = array(t);
//   };

//   module MarketCombination = {
//     type t = {
//       agentPredictions: MeasurementsWithTime.t,
//       marketPredictions: MeasurementsWithTime.t,
//       resolution: MeasurementWithTime.t,
//       beginningTime: time,