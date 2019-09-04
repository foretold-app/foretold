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
    if (Array.length(xs) != Array.length(ys)) {
      Error("Array length must be the same");
    } else {
      Ok({xs, ys});
    };

  let verifySameLength = ({xs, ys}) =>
    if (Array.length(xs) == 0 || Array.length(ys) == 0) {
      Error("Arrays must have more than 0 elements.");
    } else {
      Ok({xs, ys});
    };

  let make = ({xs, ys}) =>
    {xs, ys} |> verifyHasLength |> Rationale.Result.bind(_, verifySameLength);
};

module Percentage = {
  type t = float;

  let verifyInRange = (f: float) =>
    if (f < 0. || f > 1.) {
      Error("Arrays must have more than 0 elements.");
    } else {
      Ok(f);
    };

  let make = (t: t) => verifyInRange(t);
};

type measurement = [
  | `Cdf(Cdf.t)
  | `Float(float)
  | `Percentage(Percentage.t)
  | `Binary(bool)
  | `UnresolvableResolution(unresolvableTypes)
  | `Comment(commentTypes)
];