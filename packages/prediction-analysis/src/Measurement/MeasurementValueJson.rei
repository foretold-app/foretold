module Keys: {
  module Unresolvable: {
    let fromString:
      string => option(MeasurementValue.UnresolvableResolution.t);
  };
};

[@genType]
type t = Js.Json.t;

[@genType]
let toMeasurementValue:
  Js.Json.t => Belt.Result.t(MeasurementValue.t, string);