type t;
let make: float => Belt.Result.t(t, string);
let makeExt: float => t;
let toMeasurement: t => [> | `Percentage(t)];
let toFloat: t => float;
let inverse: t => t;
let fromBool: bool => t;