open Utils;
open Belt.Result;

let apolloResponseToResult = (result: ReasonApolloTypes.queryResponse('a)) =>
  switch (result) {
  | Loading => Error(<div> <Spin /> </div>)
  | Error(_error) =>
    Error(
      <div>
        {"Oops! Something went wrong. Try reloading this page." |> ste}
      </div>,
    )
  | Data(response) => Ok(response)
  };