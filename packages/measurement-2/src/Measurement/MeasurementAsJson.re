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
      | "AMBIGUOUS" => Ok(`Ambiguous)
      | "FALSE_CONDITIONAL" => Ok(`FalseConditional)
      | "RESULT_NOT_AVAILABLE" => Ok(`ResultNotAvailable)
      | "OTHER" => Ok(`Other)
      | _ => Error("Invalid Unresolvable Resolution: " ++ str)
      };

    let stringToComment =
        (str: string): Belt.Result.t(MeasurementValue.Comment.t, string) =>
      switch (str) {
      | "GENERIC" => Ok(`Generic)
      | "QUESTION_FEEDBACK" => Ok(`QuestionFeedback)
      | "UPDATE" => Ok(`Update)
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
      jsonToFinalValue(CustomDecoders.jsonToCdfPair, ({xs, ys}) =>
        MeasurementValue.Cdf.(
          make(~xs, ~ys, ())->Belt.Result.map(_, toMeasurement)
        )
      )
    | `Percentage =>
      jsonToFinalValue(Json.Decode.float, e =>
        MeasurementValue.Percentage.(
          e |> make |> Belt.Result.map(_, toMeasurement)
        )
      )
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
    let decodingType = MeasurementValueType.TypeName.fromString(t);
    switch (decodingType) {
    | Ok(e) => json |> decodeData(e)
    | Error(n) => Error(n)
    };
  };
};