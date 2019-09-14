type t;

module Errors: {
  type error =
    | XsIncrementing
    | YsIncrementing
    | RangeError
    | LengthGreaterThan(int)
    | LengthLessThan(int)
    | DifferentLength
    | YBetween0And1;

  type e = {
    cdf: option(t),
    error,
  };

  let toString: e => string;
};

let make:
  (
    ~xs: array(float),
    ~ys: array(float),
    ~minLength: int,
    ~maxLength: int,
    unit
  ) =>
  Belt.Result.t(t, Errors.e);

let makeWithoutValidations:
  (~xs: array(float), ~ys: array(float), unit) => t;

let toMeasurementValue: t => [> | `Cdf(t)];

let toDistribution: t => Distribution.t;