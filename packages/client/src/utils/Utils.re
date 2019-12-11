[@bs.config {jsx: 3}];

open Belt.Result;

let resolveRegex = (exp, str) => {
  let resolveOption = opt =>
    switch (opt) {
    | None => ""
    | Some(s) => s
    };

  let res = exp |> Js.Re.exec(str);
  switch (res) {
  | None => ""
  | Some(result) =>
    let captures = result |> Js.Re.captures;
    switch (captures) {
    | [|_, token|] => token |> Js.Nullable.toOption |> resolveOption
    | _ => ""
    };
  };
};

let ste = ReasonReact.string;

let filterOptionalResult = (errorMessage, result: option('a)) =>
  switch (result) {
  | Some(v) => Ok(v)
  | None => Error(errorMessage)
  };

let idd = e => e;

let filterAndFold = fn =>
  E.A.fold_left(
    (acc, elem) => fn(elem, e => Array.concat([acc, [|e|]]), () => acc),
    [||],
  );

/* TODO: Move */
let doIfSome = (fn, s: option('a)) =>
  switch (s) {
  | Some(r) => fn(r)
  | _ => ()
  };

[@bs.val] external setTimeout: (unit => unit, int) => float = "setTimeout";