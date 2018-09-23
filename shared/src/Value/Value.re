open Rationale;
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

module type Point = {
  type t;
  let equal: (t, t) => bool;
  let encodeFn: t => Js.Json.t;
  let decodeFn: Json.Decode.decoder(t);
};

module MakeByPercentile = (Item: Point) => {
  module Id =
    Belt.Id.MakeComparable({
      type t = float;
      let cmp: (float, float) => int = Pervasives.compare;
    });

  type t = Belt.Map.t(Id.t, Item.t, Id.identity);

  type itemT = Item.t;

  let toArray = (t: t) => t |> Belt.Map.toArray;

  let toDict = (t: t) =>
    t
    |> toArray
    |> Array.map(((a, b)) => (string_of_float(a), b))
    |> Js.Dict.fromArray;

  let fromArray = a => a |> Belt.Map.fromArray(~id=(module Id));

  let of_string = "stuff here";
  let fromDict = (r: Js.Dict.t(Item.t)) =>
    r
    |> Js.Dict.entries
    |> Array.map(((a, b)) => (float_of_string(a), b))
    |> Belt.Map.fromArray(~id=(module Id));

  let decode =
    decodeResult(
      Json.Decode.field("data", Json.Decode.dict(Item.decodeFn)) ||> fromDict,
    );

  let encode = (name: string, t: t) => {
    let dic = t |> toDict;
    Json.Encode.(
      object_([
        ("dataType", Json.Encode.string(name)),
        (
          "data",
          dic
          |> Js.Dict.map((. value) => Item.encodeFn(value))
          |> Json.Encode.dict,
        ),
      ])
    );
  };
};

module FloatPoint = {
  type t = float;
  let equal = (a: t, b: t) => a == b;

  let decodeFn = Json.Decode.float;
  let encodeFn = Json.Encode.float;
};

module DateTimePoint = {
  type t = string;
  let equal = (a: t, b: t) => a == b;

  let encodeFn = Json.Encode.string;
  let decodeFn = Json.Decode.string;
};

module FloatPercentiles = MakeByPercentile(FloatPoint);
module DateTimePercentiles = MakeByPercentile(DateTimePoint);

type t = [
  | `FloatPoint(float)
  | `Percentage(float)
  | `Binary(int)
  | `DateTimePoint(string)
  | `FloatPercentiles(FloatPercentiles.t)
  | `DateTimePercentiles(DateTimePercentiles.t)
];

let hasQuartiles = (t: 'a) : bool =>
  Belt.Map.has(t, 25.0) && Belt.Map.has(t, 50.0) && Belt.Map.has(t, 75.0);

let isValid = (t: t) =>
  switch (t) {
  | `FloatPoint(_) => true
  | `FloatPercentiles(i) => hasQuartiles(i)
  | `DateTimePercentiles(i) => hasQuartiles(i)
  | `Percentage(i) => 0.0 <= i && i <= 100.0
  | `Binary(i) => i == 0 || i == 1
  | `DateTimePoint(_) => true
  };

let typeToName = (t: t) =>
  switch (t) {
  | `FloatPoint(_) => "floatPoint"
  | `FloatPercentiles(_) => "floatPercentiles"
  | `DateTimePercentiles(_) => "dateTimePercentiles"
  | `DateTimePoint(_) => "dateTimePoint"
  | `Percentage(_) => "percentagePoint"
  | `Binary(_) => "binary"
  };

let nameToType =
  fun
  | "floatPoint" => Ok(`FloatPoint)
  | "floatPercentiles" => Ok(`FloatPercentiles)
  | "dateTimePoint" => Ok(`DateTimePoint)
  | "dateTimePercentiles" => Ok(`DateTimePercentiles)
  | "percentagePoint" => Ok(`Percentage)
  | "binary" => Ok(`Binary)
  | _ => Error("Not found");

let stringOfValue = (t: t) =>
  switch (t) {
  | `FloatPoint(k) => string_of_float(k)
  | `FloatPercentiles(t) =>
    let per = perc =>
      Belt.Map.getWithDefault(t, perc, 0.0) |> int_of_float |> string_of_int;
    let p25 = per(25.0);
    let p50 = per(50.0);
    let p75 = per(75.0);
    {j|{25: $(p25), 50: $(p50), 75: $(p75)} |j};
  | `DateTimePercentiles(k) => "Implement Me"
  | `DateTimePoint(k) => k
  | `Percentage(k) => string_of_float(k)
  | `Binary(k) => string_of_int(k)
  };

let encode = (e: t) => {
  let n = typeToName(e);
  switch (e) {
  | `FloatPoint(k) => makeEncode(Json.Encode.float, n, k)
  | `FloatPercentiles(k) => FloatPercentiles.encode(n, k)
  | `DateTimePercentiles(k) => DateTimePercentiles.encode(n, k)
  | `DateTimePoint(k) => makeEncode(Json.Encode.string, n, k)
  | `Percentage(k) => makeEncode(Json.Encode.float, n, k)
  | `Binary(k) => makeEncode(Json.Encode.int, n, k)
  };
};

let convert = (decoder, toValue, json) => json |> decoder <$> toValue;

let decoder = (a, j: Js.Json.t) : Belt.Result.t(t, string) =>
  switch (a) {
  | `FloatPoint =>
    j |> convert(makeDecode(Json.Decode.float), e => `FloatPoint(e))
  | `FloatPercentiles =>
    j |> convert(FloatPercentiles.decode, e => `FloatPercentiles(e))
  | `DateTimePoint =>
    j |> convert(makeDecode(Json.Decode.string), e => `DateTimePoint(e))
  | `DateTimePercentiles =>
    j |> convert(DateTimePercentiles.decode, e => `DateTimePercentiles(e))
  | `Percentage =>
    j |> convert(makeDecode(Json.Decode.float), e => `Percentage(e))
  | `Binary => j |> convert(makeDecode(Json.Decode.int), e => `Binary(e))
  };

let decode = (j: Js.Json.t) : Belt.Result.t(t, string) => {
  let t = j |> Json.Decode.field("dataType", Json.Decode.string);
  let decodingType = nameToType(t);
  switch (decodingType) {
  | Ok(e) => j |> decoder(e)
  | Error(n) => Error(n)
  };
};