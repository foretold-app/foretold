open Belt.Result;
open Rationale;

let ste = ReasonReact.string;

let apolloResponseToResult = (result: ReasonApolloTypes.queryResponse('a)) =>
  switch (result) {
  | Loading => Error(<div> ("Loading" |> ste) </div>)
  | Error(error) => Error(<div> (error##message |> ste) </div>)
  | Data(response) => Ok(response)
  };

let filterOptionalResult = (errorMessage, result: option('a)) =>
  switch (result) {
  | Some(v) => Ok(v)
  | None => Error(errorMessage)
  };

let ste = ReasonReact.string;

let idd = e => e;

let catOptionals = (optionals: Js.Array.t(option('a))) : Js.Array.t('a) =>
  optionals
  |> Js.Array.filter(Option.isSome)
  |> Js.Array.map(Option.toExn(""));