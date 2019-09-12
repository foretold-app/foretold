type t = Js.Json.t;
let toMeasurementValue:
  Js.Json.t => Belt.Result.t(MeasurementValue.t, string);