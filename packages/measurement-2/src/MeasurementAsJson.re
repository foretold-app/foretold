open Belt.Result;
type t = Js.Json.t;
open Rationale.Result.Infix;

module JsonToMeasurement = {
  module CustomDecoders = {
    let floatToPercentage = MeasurementValue.Percentage.make;

    let stringToUnresolvableResolution =
        (str: string)
        : Belt.Result.t(MeasurementValue.UnresolvableResolution.t, string) =>
      switch (str) {
      | "AMBIGUOUS" => Ok(`AMBIGUOUS)
      | "FALSE_CONDITIONAL" => Ok(`FALSE_CONDITIONAL)
      | "OTHER" => Ok(`OTHER)
      | "RESULT_NOT_AVAILABLE" => Ok(`RESULT_NOT_AVAILABLE)
      | _ => Error("Invalid Unresolvable Resolution: " ++ str)
      };

    let stringToComment =
        (str: string): Belt.Result.t(MeasurementValue.Comment.t, string) =>
      switch (str) {
      | "GENERIC" => Ok(`GENERIC)
      | "QUESTION_FEEDBACK" => Ok(`QUESTION_FEEDBACK)
      | "UPDATE" => Ok(`UPDATE)
      | _ => Error("Invalid GraphQL Comment: " ++ str)
      };

    let jsonToCdfPair = (j: Js.Json.t): MeasurementValue.Cdf.t => {
      let xs =
        j |> Json.Decode.field("xs", Json.Decode.array(Json.Decode.float));
      let ys =
        j |> Json.Decode.field("ys", Json.Decode.array(Json.Decode.float));
      {xs, ys};
    };
  };

  let jsonToSimpleValue = (fn, json: Js.Json.t) =>
    try (Ok(json |> fn)) {
    | Json_decode.DecodeError(e) => Error(e)
    | _ => Error("Unknown Error.")
    };

  let nameToType =
    fun
    | "floatPoint" => Ok(`Float)
    | "floatCdf" => Ok(`Cdf)
    | "percentage" => Ok(`Percentage)
    | "binary" => Ok(`Binary)
    | "unresolvableResolution" => Ok(`UnresolvableResolution)
    | "comment" => Ok(`Comment)
    | _ => Error("Not found");

  let decodeData =
      (a, json: Js.Json.t)
      : Belt.Result.t(MeasurementValue.MeasurementValue.t, string) => {
    let jsonToFinalValue = (toSimpleValueFn, toFinalValueFn) =>
      json
      |> jsonToSimpleValue(Json.Decode.field("data", toSimpleValueFn))
      >>= toFinalValueFn;

    switch (a) {
    | `Float => jsonToFinalValue(Json.Decode.float, e => Ok(`Float(e)))
    | `Cdf =>
      jsonToFinalValue(CustomDecoders.jsonToCdfPair, e =>
        e->MeasurementValue.Cdf.make->Belt.Result.map(r => `Cdf(r))
      )
    | `Percentage =>
      jsonToFinalValue(Json.Decode.float, e => Ok(`Percentage(e)))
    | `Binary => jsonToFinalValue(Json.Decode.bool, e => Ok(`Binary(e)))
    | `UnresolvableResolution =>
      jsonToFinalValue(Json.Decode.string, e =>
        e
        ->CustomDecoders.stringToUnresolvableResolution
        ->Belt.Result.map(r => `UnresolvableResolution(r))
      )
    | `Comment =>
      jsonToFinalValue(Json.Decode.string, e =>
        e->CustomDecoders.stringToComment->Belt.Result.map(r => `Comment(r))
      )
    };
  };

  let run =
      (json: Js.Json.t)
      : Belt.Result.t(MeasurementValue.MeasurementValue.t, string) => {
    let t = json |> Json.Decode.field("dataType", Json.Decode.string);
    let decodingType = nameToType(t);
    switch (decodingType) {
    | Ok(e) => json |> decodeData(e)
    | Error(n) => Error(n)
    };
  };
};