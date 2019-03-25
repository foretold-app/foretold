/* O for option */
open Rationale.Function.Infix;

/* Utils */
module U = {
  let isEqual = (a, b) => a == b;
  let toA = a => [|a|];
  let id = e => e;
};

module O = {
  let dimap = (sFn, rFn, e) =>
    switch (e) {
    | Some(r) => sFn(r)
    | None => rFn()
    };
  ();
  let fmap = Rationale.Option.fmap;
  let bind = Rationale.Option.bind;
  let default = Rationale.Option.default;
  let isSome = Rationale.Option.isSome;
  let toExn = Rationale.Option.toExn;

  let toResult = (error, e) =>
    switch (e) {
    | Some(r) => Belt.Result.Ok(r)
    | None => Error(error)
    };

  module React = {
    let defaultNull = default(ReasonReact.null);
    let fmapOrNull = fn => fmap(fn) ||> default(ReasonReact.null);
    let flatten = default(ReasonReact.null);
  };
};

/* Functions */
module F = {
  let apply = (a, e) => a |> e;
};

module Float = {
  let with3DigitsPrecision = Js.Float.toPrecisionWithPrecision(_, ~digits=3);
};

module I = {
  let increment = 1->(+);
  let decrement = 1->(-);
};

/* R for Result */
module R = {
  let result = Rationale.Result.result;
  let id = e => e |> result(U.id, U.id);
  let fmap = Rationale.Result.fmap;
  let bind = Rationale.Result.bind;
};

let safe_fn_of_string = (fn, s: string): option('a) =>
  try (Some(fn(s))) {
  | _ => None
  };

module S = {
  let toReact = ReasonReact.string;
  let safe_float = float_of_string->safe_fn_of_string;
  let safe_int = int_of_string->safe_fn_of_string;
};

module J = {
  let toString = Js.Json.decodeString ||> O.default("");
  let toMoment = toString ||> MomentRe.moment;
  module O = {
    let toMoment = O.fmap(toMoment);
  };
};

module M = {
  open MomentRe;
  let format = Moment.format;
  let format_standard = "MMM DD, YYYY HH:MM:SS";
  let format_simple = "L";
  /* TODO: Figure out better name */
  let goFormat_simple = format(format_simple);
};

module JsDate = {
  let fromString = Js.Date.fromString;
  let now = Js.Date.now;
  let make = Js.Date.make;
  let valueOf = Js.Date.valueOf;
};

/* List */
module L = {
  let fmap = List.map;
  let concat = List.concat;
  let find = List.find;
  let filter = List.filter;
  let for_all = List.for_all;
  let exists = List.exists;
  let sort = List.sort;
  let length = List.length;
  let filter_opt = Rationale.RList.filter_opt;
  let uniqBy = Rationale.RList.uniqBy;
  let join = Rationale.RList.join;
  let head = Rationale.RList.head;
  let uniq = Rationale.RList.uniq;
  let flatten = List.flatten;
  let last = Rationale.RList.last;
  let append = List.append;
  let getBy = Belt.List.getBy;
  let dropLast = Rationale.RList.dropLast;
  let contains = Rationale.RList.contains;
  let without = Rationale.RList.without;
  let iter = List.iter;
  let findIndex = Rationale.RList.findIndex;
};

/* A for Array */
module A = {
  let fmap = Array.map;
  let fmapi = Array.mapi;
  let to_list = Array.to_list;
  let of_list = Array.of_list;
  let length = Array.length;
  let empty = [||];
  let unsafe_get = Array.unsafe_get;
  let get = Belt.Array.get;
  let fold_left = Array.fold_left;
  let fold_right = Array.fold_right;
  let concatMany = Belt.Array.concatMany;
  let keepMap = Belt.Array.keepMap;
  let stableSortBy = Belt.SortArray.stableSortBy;
  /* TODO: Is there a better way of doing this? */

  /* TODO: Is -1 still the indicator that this is false (as is true with js findIndex)? Wasn't sure. */
  let findIndex = (e, i) =>
    Js.Array.findIndex(e, i)
    |> (
      r =>
        switch (r) {
        | (-1) => None
        | r => Some(r)
        }
    );
  let filter = (o, e) => Js.Array.filter(o, e);
  module O = {
    let concatSomes = (optionals: Js.Array.t(option('a))): Js.Array.t('a) =>
      optionals
      |> Js.Array.filter(Rationale.Option.isSome)
      |> Js.Array.map(
           Rationale.Option.toExn("Warning: This should not have happened"),
         );
    let concatSome = (optionals: array(option('a))): array('a) =>
      optionals
      |> Js.Array.filter(Rationale.Option.isSome)
      |> Js.Array.map(
           Rationale.Option.toExn("Warning: This should not have happened"),
         );
  };
};

module JsArray = {
  let concatSomes = (optionals: Js.Array.t(option('a))): Js.Array.t('a) =>
    optionals
    |> Js.Array.filter(Rationale.Option.isSome)
    |> Js.Array.map(
         Rationale.Option.toExn("Warning: This should not have happened"),
       );
  let filter = Js.Array.filter;
};

module FloatCdf = {
  type t = {
    xs: array(float),
    ys: array(float),
  };
  let empty: t = {xs: [||], ys: [||]};

  let firstAbove = (min: float, t: MeasurementValue.FloatCdf.t) =>
    Belt.Map.findFirstBy(t, (k, _v) => k > min);

  let firstAboveValue = (min: float, t: MeasurementValue.FloatCdf.t) =>
    Rationale.Option.fmap(((_, x)) => x, firstAbove(min, t));
};

module React = {
  let el = ReasonReact.element;
  let null = ReasonReact.null;
  let makeToEl = (~key="", ~children=<div />, e) =>
    children |> e |> el(~key);
  let showIf = (cond, comp) => cond ? comp : ReasonReact.null;
  let inP = e => <p> e </p>;
  let inH1 = e => <h1> e </h1>;
  let inH2 = e => <h2> e </h2>;
  let inH3 = e => <h3> e </h3>;
};