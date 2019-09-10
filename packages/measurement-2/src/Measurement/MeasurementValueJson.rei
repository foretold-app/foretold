type t = Js.Json.t;
let toMeasurement: Js.Json.t => Belt.Result.t(MeasurementValue.t, string);