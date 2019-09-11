open Belt.Result;

type t = {
  xs: array(float),
  ys: array(float),
};

let _verifyNIncreasing = (fnN, errorMessageIfNotIncreasing, t: t) =>
  fnN(t)
  |> Utility.Array.toRanges
  |> Rationale.Result.fmap(r =>
       Belt.Array.every(r, ((first, second)) => second >= first)
         ? Ok(r) : Error(errorMessageIfNotIncreasing)
     )
  |> Rationale.Result.fmap(_ => t);

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

// TODO: Verify ys are between 0 and 1.

let make = (~xs, ~ys, ~minLength=5, ~maxLength=10000, ()) =>
  {xs, ys}
  |> verifyMinLength(minLength)
  |> Rationale.Result.bind(_, verifyMaxLength(maxLength))
  |> Rationale.Result.bind(_, verifySameLength)
  |> Rationale.Result.bind(_, verifyXsIncreasing)
  |> Rationale.Result.bind(_, verifyYsIncreasing);

// Verify length is limited to specific number of points.
// TODO: Fn to shorten and to truncate.
let toMeasurement = t => `Cdf(t);