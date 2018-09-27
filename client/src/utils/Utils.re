open Belt.Result;
open Rationale;

let ste = ReasonReact.string;

let apolloResponseToResult = (result: ReasonApolloTypes.queryResponse('a)) =>
  switch (result) {
  | Loading => Error(<div> ("Loading" |> ste) </div>)
  | Error(error) => Error(<div> ("WHAT?" |> ste) </div>)
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