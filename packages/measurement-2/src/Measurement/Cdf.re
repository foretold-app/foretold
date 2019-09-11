open Belt.Result;

type t = {
  xs: array(float),
  ys: array(float),
};

module Verifications = {
  open Rationale.Result.Infix;

  let _verifyNIncreasing = (fnN, errorMessageIfNotIncreasing, t: t) =>
    fnN(t)
    |> Utility.Array.toRanges
    <$> (
      r =>
        Belt.Array.every(r, ((first, second)) => second >= first)
          ? Ok(r) : Error(errorMessageIfNotIncreasing)
    )
    <$> (_ => t);

  let verifyXsIncreasing = _verifyNIncreasing(t => t.xs, "Xs must increase");
  let verifyYsIncreasing = _verifyNIncreasing(t => t.ys, "Ys must increase");

  let verifyMinLength = (minLength, {xs, ys}) =>
    Array.length(xs) < minLength || Array.length(ys) < minLength
      ? Error("Length must be greater than:" ++ (minLength |> string_of_int))
      : Ok({xs, ys});

  let verifySameLength = ({xs, ys}) =>
    Array.length(xs) != Array.length(ys)
      ? Error("Array length must be the same.") : Ok({xs, ys});

  let verifyMaxLength = (maxLength, {xs, ys}) =>
    Array.length(xs) <= maxLength
      ? Error("Length must be less than: " ++ (maxLength |> string_of_int))
      : Ok({xs, ys});

  let verifyYsBetween0And1 = ({xs, ys}) =>
    Belt.Array.every(ys, e => e >= 0. && e <= 1.)
      ? Ok({xs, ys}) : Error("All Y values must be between 0 and 1");

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