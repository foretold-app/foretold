type t;
let make:
  (~xs: array(float), ~ys: array(float), unit) => Belt.Result.t(t, string);
let toMeasurement: t => [> | `Cdf(t)];