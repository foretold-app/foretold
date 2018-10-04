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

module MakeCdf = (Item: Point) => {
  module Id =
    Belt.Id.MakeComparable({
      type t = float;
      let cmp: (float, float) => int = Pervasives.compare;
    });

  /* type t = { */
  type t = Belt.Map.t(Id.t, Item.t, Id.identity);

  [@bs.deriving abstract]
  type point = {
    y: Id.t,
    x: Item.t,
  };

  type itemT = Item.t;

  let toArray = Belt.Map.toArray;

  let toArrays = (t: t) => (
    t |> Belt.Map.keysToArray,
    t |> Belt.Map.valuesToArray,
  );

  let toDict = (t: t) =>
    t
    |> toArray
    |> Array.map(((y, x)) => (string_of_float(y), x))
    |> Js.Dict.fromArray;

  let toPoints = (t: t) =>
    t |> toArray |> Array.map(((y, x)) => point(~x, ~y));

  let fromArray = a => a |> Belt.Map.fromArray(~id=(module Id));

  let fromDict = (r: Js.Dict.t(Item.t)) =>
    r
    |> Js.Dict.entries
    |> Array.map(((a, b)) => (float_of_string(a), b))
    |> Belt.Map.fromArray(~id=(module Id));

  /* There's some bug here but I'm not sure what it is. */
  let fromArrays = (a: (array(Item.t), array(float))) : t => {
    let (xs, ys) = a;
    Belt.Array.zip(xs, ys)
    |> Array.map(((x, y)) => (string_of_float(y), x))
    |> Js.Dict.fromArray
    |> fromDict;
  };

  let dataDecoder = (j: Js.Json.t) : Js.Dict.t(Item.t) => {
    let xs = j |> Json.Decode.field("xs", Json.Decode.array(Item.decodeFn));
    let ys =
      j |> Json.Decode.field("ys", Json.Decode.array(Json.Decode.float));
    Belt.Array.zip(xs, ys)
    |> Array.map(((a, b)) => (string_of_float(b), a))
    |> Js.Dict.fromArray;
  };

  let decode =
    decodeResult(Json.Decode.field("data", dataDecoder) ||> fromDict);

  let dataEncoder = t => {
    let (xs, ys) = t |> toArrays;
    let xsEncoded =
      xs |> Array.map(Json.Encode.float) |> Json.Encode.jsonArray;
    let ysEncoded = ys |> Array.map(Item.encodeFn) |> Json.Encode.jsonArray;
    Json.Encode.(object_([("xs", xsEncoded), ("ys", ysEncoded)]));
  };

  let encode = (name: string, t: t) =>
    Json.Encode.(
      object_([
        ("dataType", Json.Encode.string(name)),
        ("data", dataEncoder(t)),
      ])
    );
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

module FloatCdf = MakeCdf(FloatPoint);

let toPdf = (~bucketSize=10, t: FloatCdf.t) : FloatCdf.t => {
  let inChunks =
    t
    |> FloatCdf.toArray
    |> Array.to_list
    |> Belt.List.keepWithIndex(_, (_, i) => i mod bucketSize == 0)
    |> Belt.List.keepWithIndex(_, (_, i) => i != 0);
  inChunks
  |> List.mapi((i, e) => {
       let (y, x) = e;
       if (i == 0) {
         e;
       } else {
         let (oy, ox) = inChunks |> List.nth(_, i - 1);
         let derivative = (y -. oy) /. (x -. ox);
         (derivative, x);
       };
     })
  |> Array.of_list
  |> FloatCdf.fromArray;
};

module DateTimeCdf = MakeCdf(DateTimePoint);

type t = [
  | `FloatPoint(float)
  | `Percentage(float)
  | `Binary(int)
  | `DateTimePoint(string)
  | `FloatCdf(FloatCdf.t)
  | `DateTimeCdf(DateTimeCdf.t)
];

let hasQuartiles = (t: 'a) : bool =>
  Belt.Map.has(t, 25.0) && Belt.Map.has(t, 50.0) && Belt.Map.has(t, 75.0);
/* when ! hasQuartiles(i) => Some("Missing quartiles") */
let error = (t: t) : option(string) =>
  switch (t) {
  | `Percentage(i) when ! (0.0 <= i && i <= 100.0) =>
    Some("Must be between 0 and 100")
  | `Binary(i) when ! (i == 0 || i == 1) => Some("Must be 0 or 1")
  | _ => None
  };

let isValid = (t: t) =>
  switch (error(t)) {
  | Some(_) => false
  | None => true
  };

let typeToName = (t: t) =>
  switch (t) {
  | `FloatPoint(_) => "floatPoint"
  | `FloatCdf(_) => "floatCdf"
  | `DateTimeCdf(_) => "dateTimeCdf"
  | `DateTimePoint(_) => "dateTimePoint"
  | `Percentage(_) => "percentagePoint"
  | `Binary(_) => "binary"
  };

let nameToType =
  fun
  | "floatPoint" => Ok(`FloatPoint)
  | "floatCdf" => Ok(`FloatCdf)
  | "dateTimePoint" => Ok(`DateTimePoint)
  | "dateTimeCdf" => Ok(`DateTimeCdf)
  | "percentagePoint" => Ok(`Percentage)
  | "binary" => Ok(`Binary)
  | _ => Error("Not found");

let stringOfValue = (t: t) =>
  switch (t) {
  | `FloatPoint(k) => string_of_float(k)
  | `FloatCdf(t) =>
    let per = perc =>
      Belt.Map.getWithDefault(t, perc, 0.0) |> int_of_float |> string_of_int;
    let p25 = per(25.0);
    let p50 = per(50.0);
    let p75 = per(75.0);
    {j|{25: $(p25), 50: $(p50), 75: $(p75)} |j};
  | `DateTimeCdf(k) => "Implement Me"
  | `DateTimePoint(k) => k
  | `Percentage(k) => string_of_float(k)
  | `Binary(k) => string_of_int(k)
  };

let encode = (e: t) => {
  let n = typeToName(e);
  switch (e) {
  | `FloatPoint(k) => makeEncode(Json.Encode.float, n, k)
  | `FloatCdf(k) => FloatCdf.encode(n, k)
  | `DateTimeCdf(k) => DateTimeCdf.encode(n, k)
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
  | `FloatCdf => j |> convert(FloatCdf.decode, e => `FloatCdf(e))
  | `DateTimePoint =>
    j |> convert(makeDecode(Json.Decode.string), e => `DateTimePoint(e))
  | `DateTimeCdf => j |> convert(DateTimeCdf.decode, e => `DateTimeCdf(e))
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