open Belt.Result;

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

// Verify length is limited to specific number of points.
// TODO: Fn to shorten and to truncate.
let toMeasurement = t => `Cdf(t);