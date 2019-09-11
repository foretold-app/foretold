type t;

module Verifications: {
  module Err: {
    type t =
      | XsIncrementing
      | YsIncrementing
      | RangeError
      | LengthGreaterThan(int)
      | LengthLessThan(int)
      | DifferentLength
      | YBetween0And1;
    let toString: t => string;
  };
};
let make:
  (
    ~xs: array(float),
    ~ys: array(float),
    ~minLength: int=?,
    ~maxLength: int=?,
    unit
  ) =>
  Belt.Result.t(t, Verifications.Err.t);
let toMeasurement: t => [> | `Cdf(t)];