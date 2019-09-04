type unresolveableTypes = [
    | `AMBIGUOUS
    | `FALSE_CONDITIONAL
    | `OTHER
    | `RESULT_NOT_AVAILABLE
  ];

type commentTypes = [ | `GENERIC | `QUESTION_FEEDBACK | `UPDATE];

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
  type t = Belt.Map.t(Id.t, float, Id.identity);

  [@bs.deriving abstract]
  type point = {
    y: Id.t,
    x: float,
  };

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

  let fromDict = (r: Js.Dict.t(float)) =>
    r
    |> Js.Dict.entries
    |> Array.map(((a, b)) => (float_of_string(a), b))
    |> Belt.Map.fromArray(~id=(module Id));

  /* There's some bug here but I'm not sure what it is. */
  let fromArrays = (a: (array(float), array(float))): t => {
    let (xs, ys) = a;
    Belt.Array.zip(xs, ys)
    |> Array.map(((x, y)) => (string_of_float(y), x))
    |> Js.Dict.fromArray
    |> fromDict;
  };

  let dataDecoder = (j: Js.Json.t): Js.Dict.t(float) => {
    let xs = j |> Json.Decode.field("xs", Json.Decode.array(Item.decodeFn));
    let ys =
      j |> Json.Decode.field("ys", Json.Decode.array(Json.Decode.float));
    Belt.Array.zip(xs, ys)
    |> Array.map(((a, b)) => (string_of_float(b), a))
    |> Js.Dict.fromArray;
  };

  let decode =
    Utils.decodeResult(Json.Decode.field("data", dataDecoder) ||> fromDict);

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

type measurement = [
  | `FloatCdf(FloatCdf.t)
  | `FloatPoint(float)
  | `Percentage(float)
  | `Binary(bool)
  | `UnresolvableResolution(UnresolvableResolution.t)
  | `Comment(Comment.t)
];
