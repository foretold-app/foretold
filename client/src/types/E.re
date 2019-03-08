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
};

/* R for Result */
module R = {
  let id = e => e |> Rationale.Result.result(Utils.idd, Utils.idd);
};