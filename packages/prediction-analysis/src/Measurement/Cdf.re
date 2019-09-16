open Belt.Result;

type t = {
  xs: array(float),
  ys: array(float),
};

let toDistribution = ({xs, ys}: t) => Distribution.T.make(xs, ys);

module Errors = {
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

  let make = (error, t) => {error, cdf: t};

  let toString = (e: e) =>
    switch (e.error) {
    | XsIncrementing => "X should be incrementing"
    | YsIncrementing => "Y should be incrementing"
    | RangeError => "Out of range"
    | LengthGreaterThan(i) => "Length is more than " ++ (i |> string_of_int)
    | LengthLessThan(i) => "Length is less than " ++ (i |> string_of_int)
    | DifferentLength => "Xs and Ys are different lengths"
    | YBetween0And1 => "Some Y values outside 0-1 range"
    };
};

module Verifications = {
  open Rationale.Result.Infix;

  let _verifyNIncreasing = (fnN, errorMessageIfNotIncreasing, t: t) =>
    fnN(t)
    |> E.Array.toRanges
    |> Rationale.Result.bimap(
         r => r,
         _ => Errors.make(Errors.RangeError, Some(t)),
       )
    >>= (
      r =>
        Belt.Array.every(r, ((first, second)) => second >= first)
          ? Ok(t)
          : Error(Errors.make(errorMessageIfNotIncreasing, Some(t)))
    );

  let verifyXsIncreasing =
    _verifyNIncreasing(t => t.xs, Errors.XsIncrementing);

  let verifyYsIncreasing =
    _verifyNIncreasing(t => t.ys, Errors.YsIncrementing);

  let verifyMinLength = (minLength, {xs, ys}) =>
    Array.length(xs) < minLength || Array.length(ys) < minLength
      ? Error(Errors.make(LengthLessThan(minLength), Some({xs, ys})))
      : Ok({xs, ys});

  let verifySameLength = ({xs, ys}) =>
    Array.length(xs) != Array.length(ys)
      ? Error(Errors.make(DifferentLength, Some({xs, ys}))) : Ok({xs, ys});

  let verifyMaxLength = (maxLength, {xs, ys}) =>
    Array.length(xs) > maxLength
      ? Error(Errors.make(LengthGreaterThan(maxLength), Some({xs, ys})))
      : Ok({xs, ys});

  let verifyYsBetween0And1 = ({xs, ys}) =>
    Belt.Array.every(ys, e => e >= 0. && e <= 1.)
      ? Ok({xs, ys}) : Error(Errors.make(YBetween0And1, Some({xs, ys})));

  // These commented out for now, because many cdfs in prod were not valid.
  let runAll = (~xs, ~ys, ~minLength, ~maxLength, ()) =>
    {xs, ys}
    |> verifyMinLength(minLength)
    >>= verifyMaxLength(maxLength)
    >>= verifySameLength;
  // >>= verifyXsIncreasing
  // >>= verifyYsIncreasing
  // >>= verifyYsBetween0And1;
};

let make = Verifications.runAll;
let makeWithoutValidations = (~xs, ~ys) => {xs, ys};
let toPdf = (t: t) => t |> toDistribution |> Distribution.T.toPdf;
let toMeasurementValue = (t: t) => `Cdf(t);