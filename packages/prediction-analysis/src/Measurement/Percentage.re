type t = float;

let verifyInRange = (f: float): Belt.Result.t(t, string) =>
  f >= 0. && f <= 1.
    ? Ok(f)
    : Error(
        "Percentage must be between 0 and 1. Instead it is:"
        ++ Js.Float.toFixedWithPrecision(f, ~digits=2),
      );

let make = verifyInRange;
let makeWithoutValidations = t => t;
let toMeasurementValue = t => `Percentage(t);
let toFloat = t => t;
let inverse = (t: t): t => 1. -. t;
let fromBool = (b: bool) => b ? 1.0 : 0.0;