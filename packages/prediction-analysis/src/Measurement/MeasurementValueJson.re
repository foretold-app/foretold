open Belt.Result;
type t = Js.Json.t;
open Rationale.Result.Infix;

module Keys = {
  let dataType = "dataType";
  let data = "data";

  module Unresolvable = {
    open MeasurementValue.UnresolvableResolution;
    let fromString =
      fun
      | "AMBIGUOUS" => Some(Ambiguous)
      | "FALSE_CONDITIONAL" => Some(FalseConditional)
      | "RESULT_NOT_AVAILABLE" => Some(ResultNotAvailable)
      | "OTHER" => Some(Other)
      | _ => None;
  };

  module Comment = {
    open MeasurementValue.Comment;
    let fromString =
      fun
      | "GENERIC" => Some(Generic)
      | "QUESTION_FEEDBACK" => Some(QuestionFeedback)
      | "UPDATE" => Some(Update)
      | _ => None;
  };

  module Cdf = {
    let xs = "xs";
    let ys = "ys";
  };
};

module JsonToMeasurement = {
  module CustomDecoders = {
    let stringToUnresolvableResolution =
        (str: string)
        : Belt.Result.t(MeasurementValue.UnresolvableResolution.t, string) =>
      str
      |> Keys.Unresolvable.fromString
      |> Rationale.Result.ofOption("Invalid Unresolvable Resolution: " ++ str);

    let stringToComment =
        (str: string): Belt.Result.t(MeasurementValue.Comment.t, string) =>
      str
      |> Keys.Comment.fromString
      |> Rationale.Result.ofOption("Invalid Comment Resolution: " ++ str);

    let jsonToCdfPair = (j: Js.Json.t) => {
      let xs =
        j
        |> Json.Decode.field(
             Keys.Cdf.xs,
             Json.Decode.array(Json.Decode.float),
           );
      let ys =
        j
        |> Json.Decode.field(
             Keys.Cdf.ys,
             Json.Decode.array(Json.Decode.float),
           );
      (xs, ys);
    };
  };

  let jsonToSimpleValue = (fn, json: Js.Json.t) =>
    try (Ok(json |> fn)) {
    | Json_decode.DecodeError(e) => Error(e)
    | _ => Error("Unknown Error.")
    };

  let decodeData =
      (a, json: Js.Json.t): Belt.Result.t(MeasurementValue.t, string) => {
    let jsonToFinalValue = (toSimpleValueFn, toFinalValueFn) =>
      json
      |> jsonToSimpleValue(Json.Decode.field(Keys.data, toSimpleValueFn))
      >>= toFinalValueFn;

    switch (a) {
    | `Float => jsonToFinalValue(Json.Decode.float, e => Ok(`Float(e)))
    | `Cdf =>
      jsonToFinalValue(CustomDecoders.jsonToCdfPair, ((xs, ys)) =>
        Cdf.make(
          ~xs,
          ~ys,
          ~minLength=DefaultParams.Cdf.minSaveLength,
          ~maxLength=DefaultParams.Cdf.maxSaveLength,
          (),
        )
        |> Rationale.Result.bimap(r => r, Cdf.Errors.toString)
        |> Belt.Result.map(_, Cdf.toMeasurementValue)
      )
    | `Percentage =>
      jsonToFinalValue(Json.Decode.float, e =>
        Percentage.(e |> make |> Belt.Result.map(_, toMeasurementValue))
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

  let run = (json: Js.Json.t): Belt.Result.t(MeasurementValue.t, string) => {
    let t = json |> Json.Decode.field(Keys.dataType, Json.Decode.string);
    let decodingType = MeasurementValueWrapper.Name.fromString(t);
    switch (decodingType) {
    | Ok(e) => json |> decodeData(e)
    | Error(n) => Error(n)
    };
  };
};

let toMeasurementValue = JsonToMeasurement.run;