/* O for option */

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
};

/* R for Result */
module R = {
  let id = e => e |> Rationale.Result.result(Utils.idd, Utils.idd);
};

/* A for Array */
module A = {
  let fmap = Array.map;
  module Optional = {
    open Rationale;
    let concatSomes = (optionals: Js.Array.t(option('a))): Js.Array.t('a) =>
      optionals
      |> Js.Array.filter(Option.isSome)
      |> Js.Array.map(Option.toExn("Warning: This should not have happened"));
  };
};