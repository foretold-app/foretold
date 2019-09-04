open Belt.Result;
type t = Js.Json.t;
open Rationale.Result.Infix;
open Rationale.Function.Infix;

module JsonToMeasurement = {
  module CustomDecoders = {
    let floatToPercentage = Types.Percentage.make;

    let stringToUnresolvableResolution =
        (str: string): Belt.Result.t(Types.UnresolvableResolution.t, string) =>
      switch (str) {
      | "AMBIGUOUS" => Ok(`AMBIGUOUS)
      | "FALSE_CONDITIONAL" => Ok(`FALSE_CONDITIONAL)
      | "OTHER" => Ok(`OTHER)
      | "RESULT_NOT_AVAILABLE" => Ok(`RESULT_NOT_AVAILABLE)
      | _ => Error("Invalid Unresolvable Resolution: " ++ str)
      };

    let stringToComment =
        (str: string): Belt.Result.t(Types.Comment.t, string) =>
      switch (str) {
      | "GENERIC" => Ok(`GENERIC)
      | "QUESTION_FEEDBACK" => Ok(`QUESTION_FEEDBACK)
      | "UPDATE" => Ok(`UPDATE)
      | _ => Error("Invalid GraphQL Comment: " ++ str)
      };
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

  let decodeResult = (fn, json) =>
    try (Ok(json |> fn)) {
    | Json_decode.DecodeError(e) => Error(e)
    | _ => Error("Unknown Error.")
    };

  let decodeData =
      (a, json: Js.Json.t): Belt.Result.t(Types.Measurement.t, string) => {
    let convert = (decoderFn, toValue) =>
      json |> decodeResult(Json.Decode.field("data", decoderFn)) >>= toValue;

    switch (a) {
    | `Float => convert(Json.Decode.float, e => Ok(`Float(e)))
    | `Cdf => convert(Json.Decode.float, e => Ok(`Float(e)))
    | `Percentage => convert(Json.Decode.float, e => Ok(`Percentage(e)))
    | `Binary => convert(Json.Decode.bool, e => Ok(`Binary(e)))
    | `UnresolvableResolution =>
      convert(Json.Decode.string, e =>
        CustomDecoders.stringToUnresolvableResolution(e)
        |> Belt.Result.map(_, r => `UnresolvableResolution(r))
      )
    | `Comment =>
      convert(Json.Decode.string, e =>
        CustomDecoders.stringToComment(e)
        |> Belt.Result.map(_, r => `Comment(r))
      )
    };
  };

  let run = (json: Js.Json.t): Belt.Result.t(Types.Measurement.t, string) => {
    let t = json |> Json.Decode.field("dataType", Json.Decode.string);
    let decodingType = nameToType(t);
    switch (decodingType) {
    | Ok(e) => json |> decodeData(e)
    | Error(n) => Error(n)
    };
  };
};