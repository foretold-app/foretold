open Rationale.Function.Infix;
open Rationale.Result.Infix;
open Belt.Result;

module UnresolvableResolution = {
  type t = [
    | `AMBIGUOUS
    | `FALSE_CONDITIONAL
    | `OTHER
    | `RESULT_NOT_AVAILABLE
  ];

  let fromString = e =>
    switch (e) {
    | "AMBIGUOUS" => `AMBIGUOUS
    | "FALSE_CONDITIONAL" => `FALSE_CONDITIONAL
    | "OTHER" => `OTHER
    | "RESULT_NOT_AVAILABLE" => `RESULT_NOT_AVAILABLE
    | _ => Js.Exn.raiseError("Invalid GraphQL Unresolvable Resolution: " ++ e)
    };

  let toString = (e: t): string =>
    switch (e) {
    | `AMBIGUOUS => "AMBIGUOUS"
    | `FALSE_CONDITIONAL => "FALSE_CONDITIONAL"
    | `OTHER => "OTHER"
    | `RESULT_NOT_AVAILABLE => "RESULT_NOT_AVAILABLE"
    };

  let toPublicString = (e: t): string =>
    switch (e) {
    | `AMBIGUOUS => "Ambiguous"
    | `FALSE_CONDITIONAL => "False Conditional"
    | `OTHER => "Other"
    | `RESULT_NOT_AVAILABLE => "Result Not Available"
    };
};

module Comment = {
  type t = [ | `GENERIC | `QUESTION_FEEDBACK | `UPDATE];

  let fromString = e =>
    switch (e) {
    | "GENERIC" => `GENERIC
    | "QUESTION_FEEDBACK" => `QUESTION_FEEDBACK
    | "UPDATE" => `UPDATE
    | _ => Js.Exn.raiseError("Invalid GraphQL Comment: " ++ e)
    };

  let toString = (e: t): string =>
    switch (e) {
    | `GENERIC => "GENERIC"
    | `QUESTION_FEEDBACK => "QUESTION_FEEDBACK"
    | `UPDATE => "UPDATE"
    };

  let toPublicString = (e: t): string =>
    switch (e) {
    | `GENERIC => "Generic"
    | `QUESTION_FEEDBACK => "Question Feedback"
    | `UPDATE => "Update"
    };
};

let decodeResult = (fn, json) =>
  try(Ok(json |> fn)) {
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
    |> Array.map(((y, x)) => (Js.Float.toString(y), x))
    |> Js.Dict.fromArray;

  let fromArray = a => a |> Belt.Map.fromArray(~id=(module Id));

  let sortArrayByXs = tArray =>
    tArray
    |> Belt.SortArray.stableSortBy(_, ((_, x1), (_, x2)) =>
         Item.compare(x1, x2)
       );

  let toPoints = (t: t) =>
    t |> toArray |> sortArrayByXs |> Array.map(((y, x)) => point(~x, ~y));

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
    |> Array.map(((x, y)) => (Js.Float.toString(y), x))
    |> Js.Dict.fromArray
    |> fromDict;
  };

  let dataDecoder = (j: Js.Json.t): Js.Dict.t(Item.t) => {
    let xs = j |> Json.Decode.field("xs", Json.Decode.array(Item.decodeFn));
    let ys =
      j |> Json.Decode.field("ys", Json.Decode.array(Json.Decode.float));
    Belt.Array.zip(xs, ys)
    |> Array.map(((a, b)) => (Js.Float.toString(b), a))
    |> Js.Dict.fromArray;
  };

  let decode =
    decodeResult(Json.Decode.field("data", dataDecoder) ||> fromDict);

  let xs = t => {
    let (_, xs) = t |> toArrays;
    xs;
  };

  let ys = t => {
    let (ys, _) = t |> toArrays;
    ys;
  };

  let xsEncoded = t =>
    t |> xs |> Array.map(Item.encodeFn) |> Json.Encode.jsonArray;

  let ysEncoded = t =>
    t |> ys |> Array.map(Json.Encode.float) |> Json.Encode.jsonArray;

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

  let encodeFn = Json.Encode.float;
  let decodeFn = Json.Decode.float;
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

type t = [
  | `FloatCdf(FloatCdf.t)
  | `FloatPoint(float)
  | `Percentage(float)
  | `Binary(bool)
  | `UnresolvableResolution(UnresolvableResolution.t)
  | `Comment(Comment.t)
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
  | `Percentage(_) => "percentage"
  | `Binary(_) => "binary"
  | `UnresolvableResolution(_) => "unresolvableResolution"
  | `Comment(_) => "comment"
  };

let nameToType =
  fun
  | "floatPoint" => Ok(`FloatPoint)
  | "floatCdf" => Ok(`FloatCdf)
  | "percentage" => Ok(`Percentage)
  | "binary" => Ok(`Binary)
  | "unresolvableResolution" => Ok(`UnresolvableResolution)
  | "comment" => Ok(`Comment)
  | _ => Error("Not found");

let stringOfValue = (t: t) =>
  switch (t) {
  | `FloatPoint(k) => Js.Float.toString(k)
  | `FloatCdf(t) =>
    let per = perc =>
      Belt.Map.getWithDefault(t, perc, 0.0) |> int_of_float |> string_of_int;
    let p25 = per(25.0);
    let p50 = per(50.0);
    let p75 = per(75.0);
    {j|{25: $(p25), 50: $(p50), 75: $(p75)} |j};
  | `Percentage(k) => Js.Float.toString(k)
  | `Binary(k) => string_of_bool(k)
  | `UnresolvableResolution(k) => UnresolvableResolution.toString(k)
  | `Comment(k) => Comment.toString(k)
  };

let encode = (e: t) => {
  let n = typeToName(e);
  switch (e) {
  | `FloatPoint(k) => makeEncode(Json.Encode.float, n, k)
  | `FloatCdf(k) => FloatCdf.encode(n, k)
  | `Percentage(k) => makeEncode(Json.Encode.float, n, k)
  | `Binary(k) => makeEncode(Json.Encode.bool, n, k)
  | `UnresolvableResolution(_k) =>
    makeEncode(Json.Encode.string, n, stringOfValue(e))
  | `Comment(_k) => makeEncode(Json.Encode.string, n, stringOfValue(e))
  };
};

let convert = (decoder, toValue, json) => json |> decoder <$> toValue;

let decoder = (a, j: Js.Json.t): Belt.Result.t(t, string) =>
  switch (a) {
  | `FloatPoint =>
    j |> convert(makeDecode(Json.Decode.float), e => `FloatPoint(e))
  | `FloatCdf => j |> convert(FloatCdf.decode, e => `FloatCdf(e))
  | `Percentage =>
    j |> convert(makeDecode(Json.Decode.float), e => `Percentage(e))
  | `Binary => j |> convert(makeDecode(Json.Decode.bool), e => `Binary(e))
  | `UnresolvableResolution =>
    j
    |> convert(makeDecode(Json.Decode.string), e =>
         `UnresolvableResolution(UnresolvableResolution.fromString(e))
       )
  | `Comment =>
    j
    |> convert(makeDecode(Json.Decode.string), e =>
         `Comment(Comment.fromString(e))
       )
  };

let decode = (j: Js.Json.t): Belt.Result.t(t, string) => {
  let t = j |> Json.Decode.field("dataType", Json.Decode.string);
  let decodingType = nameToType(t);
  switch (decodingType) {
  | Ok(e) => j |> decoder(e)
  | Error(n) => Error(n)
  };
};

let decodeGraphql = (j): Belt.Result.t(t, string) =>
  switch (
    j##floatCdf,
    j##floatPoint,
    j##percentage,
    j##binary,
    j##unresolvableResolution,
    j##comment,
  ) {
  | (Some(r), _, _, _, _, _) =>
    Ok(`FloatCdf(FloatCdf.fromArrays((r##xs, r##ys))))
  | (_, Some(r), _, _, _, _) => Ok(`FloatPoint(r))
  | (_, _, Some(r), _, _, _) => Ok(`Percentage(r))
  | (_, _, _, Some(r), _, _) => Ok(`Binary(r))
  | (_, _, _, _, Some(r), _) => Ok(`UnresolvableResolution(r))
  | (_, _, _, _, _, Some(r)) => Ok(`Comment(r))
  | _ => Error("Could not convert")
  };

let encodeToGraphQLMutation = (e: t) => {
  switch (e) {
  | `FloatPoint(k) =>
    Some({
      "floatPoint": Some(k),
      "floatCdf": None,
      "percentage": None,
      "binary": None,
      "unresolvableResolution": None,
      "comment": None,
    })
  | `FloatCdf(k) =>
    Some({
      "floatPoint": None,
      "floatCdf": Some({"xs": FloatCdf.xs(k), "ys": FloatCdf.ys(k)}),
      "percentage": None,
      "binary": None,
      "unresolvableResolution": None,
      "comment": None,
    })
  | `Percentage(k) =>
    Some({
      "floatPoint": None,
      "floatCdf": None,
      "percentage": Some(k),
      "binary": None,
      "unresolvableResolution": None,
      "comment": None,
    })
  | `Binary(k) =>
    Some({
      "floatPoint": None,
      "floatCdf": None,
      "percentage": None,
      "binary": Some(k),
      "unresolvableResolution": None,
      "comment": None,
    })
  | `UnresolvableResolution(k) =>
    Some({
      "floatPoint": None,
      "floatCdf": None,
      "percentage": None,
      "binary": None,
      "unresolvableResolution": Some(k),
      "comment": None,
    })
  | `Comment(k) =>
    Some({
      "floatPoint": None,
      "floatCdf": None,
      "percentage": None,
      "binary": None,
      "unresolvableResolution": None,
      "comment": Some(k),
    })
  };
};