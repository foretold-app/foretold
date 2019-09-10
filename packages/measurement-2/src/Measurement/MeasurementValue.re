open Belt.Result;

module Comment = {
  type t = [ | `Generic | `QuestionFeedback | `Update];

  let toString = (e: t): string =>
    switch (e) {
    | `Generic => "Generic"
    | `QuestionFeedback => "Question Feedback"
    | `Update => "Update"
    };

  let toMeasurement = (t: t) => `Comment(t);
};

module UnresolvableResolution = {
  type t = [ | `Ambiguous | `FalseConditional | `ResultNotAvailable | `Other];

  let toString = (e: t): string =>
    switch (e) {
    | `Ambiguous => "Ambiguous"
    | `FalseConditional => "FalseConditional"
    | `ResultNotAvailable => "ResultNotAvailable"
    | `Other => "Other"
    };
  let toMeasurement = (t: t) => `UnresolvableResolution(t);
};

module type Cdf = {
  type t;
  let make:
    (~xs: array(float), ~ys: array(float)) => Belt.Result.t(t, string);
  let toMeasurement: t => [> | `Cdf(t)];
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
  let make = (~xs, ~ys, ()) =>
    {xs, ys} |> verifyHasLength |> Rationale.Result.bind(_, verifySameLength);

  let toMeasurement = t => `Cdf(t);
};

module type Percentage = {
  type t;
  let make: float => Belt.Result.t(t, string);
  let makeExt: float => t;
  let toMeasurement: t => [> | `Percentage(t)];
  let toFloat: t => float;
  let inverse: t => t;
  let fromBool: bool => t;
};

module Percentage: Percentage = {
  type t = float;

  let verifyInRange = (f: float): Belt.Result.t(t, string) =>
    if (f < 0. || f > 1.) {
      Error("Percentage must be between 0 and 1.");
    } else {
      Ok(f: t);
    };

  let make = verifyInRange;
  let makeExt = t => t;
  let toMeasurement = t => `Percentage(t);
  let toFloat = t => t;
  let inverse = (t: t): t => 1. -. t;
  let fromBool = (b: bool) => b ? 1.0 : 0.0;
};

type t =
  MeasurementValueWrapper.T.t(
    Cdf.t,
    float,
    bool,
    Percentage.t,
    UnresolvableResolution.t,
    Comment.t,
  );