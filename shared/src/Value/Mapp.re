open Rationale;

module type Point = {
  type t;
  let equal: (t, t) => bool;
  let isValid: t => bool;
  let encodeFn: t => Js.Json.t;
  let decodeFn: Json.Decode.decoder(t);
};

type percentile = float;
let allTrue = e => Array.fold_left((x, y) => x && y, true, e);

module MakeByPercentile = (Item: Point) => {
  module Id =
    Belt.Id.MakeComparable({
      type t = float;
      let cmp: (float, float) => int = Pervasives.compare;
    });

  type t = Belt.Map.t(Id.t, Item.t, Id.identity);

  let hasQuartiles = (t: t) : bool =>
    Belt.Map.has(t, 25.0) && Belt.Map.has(t, 50.0) && Belt.Map.has(t, 75.0);
  let toDict = (t: t) =>
    t
    |> Belt.Map.toArray
    |> Array.map(((a, b)) => (string_of_float(a), b))
    |> Js.Dict.fromArray;

  let fromDict = (r: Js.Dict.t(Item.t)) =>
    r
    |> Js.Dict.entries
    |> Array.map(((a, b)) => (float_of_string(a), b))
    |> Belt.Map.fromArray(~id=(module Id));
  /* Json.Decode.float */
  let decode = json =>
    json
    |> Json.Decode.field("data", Json.Decode.dict(Item.decodeFn))
    |> fromDict;
  let encode = (t: t) => {
    let dic = t |> toDict;
    Json.Encode.(
      object_([
        ("dataType", Json.Encode.string("MakeByPercentile")),
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
  let decodeType = Json.Decode.float;
  let name = "floatPoint";
  let equal = (a: t, b: t) => a == b;
  let isValid = _ => true;

  let decode = json => json |> Json.Decode.field("data", Json.Decode.float);
  let encode = (i: t) =>
    Json.Encode.(
      object_([
        ("dataType", Json.Encode.string(name)),
        ("data", Json.Encode.float(i)),
      ])
    );
  let encodeFn = Json.Encode.float;
  let decodeFn = Json.Decode.float;
};

module DateTimePoint = {
  type t = string;
  let decodeType = Json.Decode.string;
  let name = "dateTimePoint";
  let equal = (a: t, b: t) => a == b;
  let isValid = (_: t) => true;
  let decode = json => json |> Json.Decode.field("data", Json.Decode.string);
  let encode = (i: t) =>
    Json.Encode.(
      object_([
        ("dataType", Json.Encode.string(name)),
        ("data", Json.Encode.string(i)),
      ])
    );

  let encodeFn = Json.Encode.string;
  let decodeFn = Json.Decode.string;
};

module FloatPercentiles = MakeByPercentile(FloatPoint);

module DateTimePercentiles = MakeByPercentile(DateTimePoint);

module Percentage = {
  type t = float;
  let name = "percentage";
  let isValid = (i: t) => i <= 0.0 && i <= 100.0;
  let decode = json => json |> Json.Decode.field("data", Json.Decode.float);
  let encode = (i: t) =>
    Json.Encode.(
      object_([
        ("dataType", Json.Encode.string(name)),
        ("data", Json.Encode.float(i)),
      ])
    );
};

module Binary = {
  type t = int;
  let name = "binary";
  let isValid = (i: t) => i == 0 || i == 1;

  let decode = json => json |> Json.Decode.field("data", Json.Decode.int);
  let encode = (i: t) =>
    Json.Encode.(
      object_([
        ("dataType", Json.Encode.string(name)),
        ("data", Json.Encode.int(i)),
      ])
    );
};

type value =
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
  | _ => FloatPoint(0.1)
  };
};

let encode = (v: value) =>
  switch (v) {
  | FloatPoint(f) => FloatPoint.encode(f)
  | Percentage(f) => Percentage.encode(f)
  | Binary(f) => Binary.encode(f)
  | DateTimePoint(f) => DateTimePoint.encode(f)
  };

let data = {| {
    "dataType": "binary",
    "dynamics": { "0.25": 5.0, "50.0": 8.0 }
  } |};

let bar = decode(data |> Json.parseOrRaise);