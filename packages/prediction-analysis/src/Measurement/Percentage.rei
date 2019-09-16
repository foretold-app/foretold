type t;
let make: float => Belt.Result.t(t, string);
let makeWithoutValidations: float => t;
let toMeasurementValue: t => [> | `Percentage(t)];
let toFloat: t => float;
let inverse: t => t;
let fromBool: bool => t;