open Rationale;

module type Point = {
  type t;
  let equal: (t, t) => bool;
  let isValid: t => bool;
  let name: string;
  let itemName: string;
  let encodeFn: t => Js.Json.t;
  let decodeFn: Json.Decode.decoder(t);
};

type percentile = float;
let allTrue = e => Array.fold_left((x, y) => x && y, true, e);

let data = "data";
let dataType = "dataType";

module MakeByPercentile = (Item: Point) => {
  module Id =
    Belt.Id.MakeComparable({
      type t = float;
      let cmp: (float, float) => int = Pervasives.compare;
    });

  type t = Belt.Map.t(Id.t, Item.t, Id.identity);

  let name = Item.itemName ++ "ByPercentile";

  let hasQuartiles = (t: t) : bool =>
    Belt.Map.has(t, 25.0) && Belt.Map.has(t, 50.0) && Belt.Map.has(t, 75.0);

  let toArray = (t: t) => t |> Belt.Map.toArray;

  let toDict = (t: t) =>
    t
    |> toArray
    |> Array.map(((a, b)) => (string_of_float(a), b))
    |> Js.Dict.fromArray;

  let fromArray = a => a |> Belt.Map.fromArray(~id=(module Id));

  let fromDict = (r: Js.Dict.t(Item.t)) =>
    r
    |> Js.Dict.entries
    |> Array.map(((a, b)) => (float_of_string(a), b))
    |> Belt.Map.fromArray(~id=(module Id));

  let decode = json =>
    json
    |> Json.Decode.field("data", Json.Decode.dict(Item.decodeFn))
    |> fromDict;

  let encode = (t: t) => {
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

let makeDecode = (decodeFn, json) =>
  json |> Json.Decode.field("data", decodeFn);

let makeEncode = (encodeFn, name: string, i: 'a) =>
  Json.Encode.(
    object_([
      ("dataType", Json.Encode.string(name)),
      ("data", encodeFn(i)),
    ])
  );

module FloatPoint = {
  type t = float;
  let itemName = "float";
  let name = itemName ++ "Point";
  let equal = (a: t, b: t) => a == b;
  let isValid = _ => true;

  let decodeFn = Json.Decode.float;
  let encodeFn = Json.Encode.float;
  let decode = makeDecode(decodeFn);
  let encode = makeEncode(encodeFn, name);
};

module DateTimePoint = {
  type t = string;
  let itemName = "dateTime";
  let name = itemName ++ "Point";
  let equal = (a: t, b: t) => a == b;
  let isValid = (_: t) => true;

  let encodeFn = Json.Encode.string;
  let decodeFn = Json.Decode.string;
  let decode = makeDecode(decodeFn);
  let encode = makeEncode(encodeFn, name);
};

module FloatPercentiles = MakeByPercentile(FloatPoint);

module DateTimePercentiles = MakeByPercentile(DateTimePoint);

module Percentage = {
  type t = float;
  let name = "percentage";
  let isValid = (i: t) => i <= 0.0 && i <= 100.0;

  let decodeFn = Json.Decode.float;
  let encodeFn = Json.Encode.float;
  let decode = makeDecode(decodeFn);
  let encode = makeEncode(encodeFn, name);
};

module Binary = {
  type t = int;
  let name = "binary";
  let isValid = (i: t) => i == 0 || i == 1;

  let decodeFn = Json.Decode.int;
  let encodeFn = Json.Encode.int;
  let decode = makeDecode(decodeFn);
  let encode = makeEncode(encodeFn, name);
};

type t =
  | FloatPoint(FloatPoint.t)
  | FloatPercentiles(FloatPercentiles.t)
  | DateTimePoint(DateTimePoint.t)
  | DateTimePercentiles(DateTimePercentiles.t)
  | Percentage(Percentage.t)
  | Binary(Binary.t);

let decode = json => {
  let t = json |> Json.Decode.field("dataType", Json.Decode.string);
  switch (t) {
  | _ when t == FloatPoint.name => FloatPoint(FloatPoint.decode(json))
  | _ when t == DateTimePoint.name =>
    DateTimePoint(DateTimePoint.decode(json))
  | _ when t == Percentage.name => Percentage(Percentage.decode(json))
  | _ when t == Binary.name => Binary(Binary.decode(json))
  | _ when t == DateTimePercentiles.name =>
    DateTimePercentiles(DateTimePercentiles.decode(json))
  | _ when t == FloatPercentiles.name =>
    FloatPercentiles(FloatPercentiles.decode(json))
  | _ => FloatPoint(0.1)
  };
};

let encodee = (t: t) =>
  switch (t) {
  | FloatPoint(f) => FloatPoint.encode(f)
  | DateTimePoint(f) => DateTimePoint.encode(f)
  | DateTimePercentiles(f) => DateTimePercentiles.encode(f)
  | FloatPercentiles(f) => FloatPercentiles.encode(f)
  | Percentage(f) => Percentage.encode(f)
  | Binary(f) => Binary.encode(f)
  };