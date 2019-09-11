open Belt.Result;

type t = {
  xs: array(float),
  ys: array(float),
};

module Verifications = {
  module Err = {
    type t =
      | XsIncrementing
      | YsIncrementing
      | RangeError
      | LengthGreaterThan(int)
      | LengthLessThan(int)
      | DifferentLength
      | YBetween0And1;

    let toString =
      fun
      | XsIncrementing => "X should be incrementing"
      | YsIncrementing => "Y should be incrementing"
      | RangeError => "Out of range"
      | LengthGreaterThan(i) => "Length is more than " ++ (i |> string_of_int)
      | LengthLessThan(i) => "Length is less than " ++ (i |> string_of_int)
      | DifferentLength => "Xs and Ys are different lengths"
      | YBetween0And1 => "Some Y values outside 0-1 range";
  };

  open Rationale.Result.Infix;

  let _verifyNIncreasing = (fnN, errorMessageIfNotIncreasing, t: t) =>
    fnN(t)
    |> Utility.Array.toRanges
    |> Rationale.Result.bimap(r => r, _ => Err.RangeError)
    <$> (
      r =>
        Belt.Array.every(r, ((first, second)) => second >= first)
          ? Ok(r) : Error(errorMessageIfNotIncreasing)
    )
    <$> (_ => t);

  let verifyXsIncreasing = _verifyNIncreasing(t => t.xs, Err.XsIncrementing);
  let verifyYsIncreasing = _verifyNIncreasing(t => t.ys, Err.YsIncrementing);

  let verifyMinLength = (minLength, {xs, ys}) =>
    Array.length(xs) < minLength || Array.length(ys) < minLength
      ? Error(Err.LengthLessThan(minLength)) : Ok({xs, ys});

  let verifySameLength = ({xs, ys}) =>
    Array.length(xs) != Array.length(ys)
      ? Error(Err.DifferentLength) : Ok({xs, ys});

  let verifyMaxLength = (maxLength, {xs, ys}) =>
    Array.length(xs) <= maxLength
      ? Error(Err.LengthGreaterThan(maxLength)) : Ok({xs, ys});

  let verifyYsBetween0And1 = ({xs, ys}) =>
    Belt.Array.every(ys, e => e >= 0. && e <= 1.)
      ? Ok({xs, ys}) : Error(Err.YBetween0And1);

  let runAll = (~xs, ~ys, ~minLength=5, ~maxLength=10000, ()) =>
    {xs, ys}
    |> verifyMinLength(minLength)
    >>= verifyMaxLength(maxLength)
    >>= verifySameLength
    >>= verifyXsIncreasing
    >>= verifyYsIncreasing
    >>= verifyYsBetween0And1;
};

let make = Verifications.runAll;

let toMeasurement = t => `Cdf(t);