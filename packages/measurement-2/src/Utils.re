open Rationale.Function.Infix;
open Rationale.Result.Infix;
open Belt.Result;

let decodeResult = (fn, json) =>
  try (Ok(json |> fn)) {
  | Json_decode.DecodeError(e) => Error(e)
  | _ => Error("Unknown Error.")
  };

let makeDecode = decodeFn =>
  decodeResult(Json.Decode.field("data", decodeFn));

let makeEncode = (encodeFn, name: string, i: 'a) =>
  Json.Encode.(
    object_([
      ("dataType", Json.Encode.string(name)),
      ("data", encodeFn(i)),
    ])
  );
