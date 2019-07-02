open Rationale.Function.Infix;
open Rationale.Result.Infix;
open Belt.Result;

type valueResult = {
  .
  "floatCdf":
    option({
      .
      "xs": Js.Array.t(float),
      "ys": Js.Array.t(float),
    }),
  "floatPoint": option(float),
  "percentage": option(float),
  "binary": option(bool),
};

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
  let compare: (t, t) => int;
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

  let fromArray = a => a |> Belt.Map.fromArray(~id=(module Id));

  let sortArrayByXs = tArray =>
    tArray
    |> Belt.SortArray.stableSortBy(_, ((_, x1), (_, x2)) =>
         Item.compare(x1, x2)
       );

  let toPoints = (t: t) =>
    t |> toArray |> sortArrayByXs |> Array.map(((y, x)) => point(~x, ~y));

  let toJss = (t: t) => {
    "ys": t |> Belt.Map.keysToArray,
    "xs": t |> Belt.Map.valuesToArray,
  };

  let toJs = (t: t) => {
    let points = t |> toArray |> sortArrayByXs;
    {
      "xs": points |> Array.map(((_, x)) => x),
      "ys": points |> Array.map(((y, _)) => y),
    };
  };

  let fromDict = (r: Js.Dict.t(Item.t)) =>
    r
    |> Js.Dict.entries
    |> Array.map(((a, b)) => (float_of_string(a), b))
    |> Belt.Map.fromArray(~id=(module Id));

  /* There's some bug here but I'm not sure what it is. */
  let fromArrays = (a: (array(Item.t), array(float))): t => {
    let (xs, ys) = a;
    Belt.Array.zip(xs, ys)
    |> Array.map(((x, y)) => (string_of_float(y), x))
    |> Js.Dict.fromArray
    |> fromDict;
  };

  let dataDecoder = (j: Js.Json.t): Js.Dict.t(Item.t) => {
    let xs = j |> Json.Decode.field("xs", Json.Decode.array(Item.decodeFn));
    let ys =
      j |> Json.Decode.field("ys", Json.Decode.array(Json.Decode.float));
    Belt.Array.zip(xs, ys)
    |> Array.map(((a, b)) => (string_of_float(b), a))
    |> Js.Dict.fromArray;
  };

  let decode =
    decodeResult(Json.Decode.field("data", dataDecoder) ||> fromDict);

  let xs = t => {
    let (xs, _) = t |> toArrays;
    xs;
  };

  let ys = t => {
    let (_, ys) = t |> toArrays;
    ys;
  };

  let xsEncoded = t =>
    t |> xs |> Array.map(Json.Encode.float) |> Json.Encode.jsonArray;

  let ysEncoded = t =>
    t |> ys |> Array.map(Item.encodeFn) |> Json.Encode.jsonArray;

  let dataEncoder = t =>
    Json.Encode.(object_([("xs", xsEncoded(t)), ("ys", ysEncoded(t))]));

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
  let compare = (a: t, b: t) => a > b ? 1 : (-1);

  let decodeFn = Json.Decode.float;
  let encodeFn = Json.Encode.float;
};

module DateTimePoint = {
  type t = string;
  let equal = (a: t, b: t) => a == b;
  let compare = (a: t, b: t) => a > b ? 1 : (-1);

  let encodeFn = Json.Encode.string;
  let decodeFn = Json.Decode.string;
};

module FloatCdf = MakeCdf(FloatPoint);

let toPdf = (t: FloatCdf.t): FloatCdf.t => {
  let inChunks = t |> FloatCdf.toArray |> Array.to_list;
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
  | `Binary(bool)
  | `DateTimePoint(string)
  | `FloatCdf(FloatCdf.t)
  | `DateTimeCdf(DateTimeCdf.t)
];

let hasQuartiles = (t: 'a): bool =>
  Belt.Map.has(t, 25.0) && Belt.Map.has(t, 50.0) && Belt.Map.has(t, 75.0);

let error = (t: t): option(string) =>
  switch (t) {
  | `Percentage(i) when !(0.0 <= i && i <= 100.0) =>
    Some("Must be between 0 and 100")
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
  | `Percentage(_) => "percentage"
  | `Binary(_) => "binary"
  };

let nameToType =
  fun
  | "floatPoint" => Ok(`FloatPoint)
  | "floatCdf" => Ok(`FloatCdf)
  | "dateTimePoint" => Ok(`DateTimePoint)
  | "dateTimeCdf" => Ok(`DateTimeCdf)
  | "percentage" => Ok(`Percentage)
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
  | `Binary(k) => string_of_bool(k)
  };

let encode = (e: t) => {
  let n = typeToName(e);
  switch (e) {
  | `FloatPoint(k) => makeEncode(Json.Encode.float, n, k)
  | `FloatCdf(k) => FloatCdf.encode(n, k)
  | `DateTimeCdf(k) => DateTimeCdf.encode(n, k)
  | `DateTimePoint(k) => makeEncode(Json.Encode.string, n, k)
  | `Percentage(k) => makeEncode(Json.Encode.float, n, k)
  | `Binary(k) => makeEncode(Json.Encode.bool, n, k)
  };
};

let convert = (decoder, toValue, json) => json |> decoder <$> toValue;

let decoder = (a, j: Js.Json.t): Belt.Result.t(t, string) =>
  switch (a) {
  | `FloatPoint =>
    j |> convert(makeDecode(Json.Decode.float), e => `FloatPoint(e))
  | `FloatCdf => j |> convert(FloatCdf.decode, e => `FloatCdf(e))
  | `DateTimePoint =>
    j |> convert(makeDecode(Json.Decode.string), e => `DateTimePoint(e))
  | `DateTimeCdf => j |> convert(DateTimeCdf.decode, e => `DateTimeCdf(e))
  | `Percentage =>
    j |> convert(makeDecode(Json.Decode.float), e => `Percentage(e))
  | `Binary => j |> convert(makeDecode(Json.Decode.bool), e => `Binary(e))
  };

let decode = (j: Js.Json.t): Belt.Result.t(t, string) => {
  let t = j |> Json.Decode.field("dataType", Json.Decode.string);
  let decodingType = nameToType(t);
  switch (decodingType) {
  | Ok(e) => j |> decoder(e)
  | Error(n) => Error(n)
  };
};

type graphQlResult = {
  .
  "floatCdf":
    option({
      .
      "xs": Js.Array.t(float),
      "ys": Js.Array.t(float),
    }),
  "floatPoint": option(float),
  "percentage": option(float),
  "binary": option(bool),
};

let decodeGraphql = (j: valueResult): Belt.Result.t(t, string) =>
  switch (j##floatCdf, j##floatPoint, j##percentage, j##binary) {
  | (Some(r), _, _, _) =>
    Ok(`FloatCdf(FloatCdf.fromArrays((r##xs, r##ys))))
  | (_, Some(r), _, _) => Ok(`FloatPoint(r))
  | (_, _, Some(r), _) => Ok(`Percentage(r))
  | (_, _, _, Some(r)) => Ok(`Binary(r))
  | _ => Error("Could not convert")
  };

let encodeToGraphQLMutation = (e: t) => {
  let n = typeToName(e);
  switch (e) {
  | `FloatPoint(k) =>
    Some({
      "floatPoint": Some(k),
      "floatCdf": None,
      "percentage": None,
      "binary": None,
    })
  | `FloatCdf(k) =>
    Some({
      "floatPoint": None,
      "floatCdf":
        Some({
          "xs": FloatCdf.xs(k) |> Array.map(e => Some(e)),
          "ys": FloatCdf.ys(k) |> Array.map(e => Some(e)),
        }),
      "percentage": None,
      "binary": None,
    })
  | `Percentage(k) =>
    Some({
      "floatPoint": None,
      "floatCdf": None,
      "percentage": Some(k),
      "binary": None,
    })
  | `Binary(k) =>
    Some({
      "floatPoint": None,
      "floatCdf": None,
      "percentage": None,
      "binary": Some(k),
    })
  | _ => None
  };
};