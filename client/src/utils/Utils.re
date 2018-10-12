open Belt.Result;
open Rationale;

let resolveOption = opt =>
  switch (opt) {
  | None => ""
  | Some(s) => s
  };

let resolveRegex = (exp, str) => {
  let res = exp |> Js.Re.exec(str);
  switch (res) {
  | None => ""
  | Some(result) =>
    let captures = result |> Js.Re.captures;
    switch (captures) {
    | [|_, token|] => token |> Js.Nullable.to_opt |> resolveOption
    | _ => ""
    };
  };
};

let ste = ReasonReact.string;

let apolloResponseToResult = (result: ReasonApolloTypes.queryResponse('a)) =>
  switch (result) {
  | Loading => Error(<div> ("Loading" |> ste) </div>)
  | Error(error) =>
    Js.log(error);
    Error(<div> ("Error: " ++ error##message |> ste) </div>);
  | Data(response) => Ok(response)
  };

let filterOptionalResult = (errorMessage, result: option('a)) =>
  switch (result) {
  | Some(v) => Ok(v)
  | None => Error(errorMessage)
  };

let ste = ReasonReact.string;

let idd = e => e;

let filterAndFold = fn =>
  Array.fold_left(
    (acc, elem) => fn(elem, e => Array.concat([acc, [|e|]]), () => acc),
    [||],
  );