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