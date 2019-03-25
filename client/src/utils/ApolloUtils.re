open Utils;
open Belt.Result;

let apolloResponseToResult = (result: ReasonApolloTypes.queryResponse('a)) =>
  switch (result) {
  | Loading => Error(<div> {"Loading" |> ste} </div>)
  | Error(error) =>
    switch (error##message) {
    | "GraphQL error: JsonWebTokenError"
    | "GraphQL error: TokenExpiredError" =>
      Contexts.Auth.Actions.logout();
      Js.log("Automatically logged out due to GraphQL Error.");
    | _ => ()
    };
    Error(
      <div>
        {"Error: " ++ error##message ++ ". Try reloading this page." |> ste}
      </div>,
    );
  | Data(response) => Ok(response)
  };