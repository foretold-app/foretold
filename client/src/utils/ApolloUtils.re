open Utils;
open Belt.Result;

let apolloResponseToResult = (result: ReasonApolloTypes.queryResponse('a)) =>
  switch (result) {
  | Loading => Error(<div> {"Loading..." |> ste} </div>)
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

module QResponse = {
  /* type t('a) = ReasonApolloTypes.queryResponse('a); */
  type tri('a) =
    | Loading
    | Error(string)
    | RelevantDataMissing
    | Success('a);

  let fromApollo = (b: ReasonApolloTypes.queryResponse('a)) =>
    switch (b) {
    | Loading => Loading
    | Error(_) => Error("sdf")
    | Data(c) => Success(c)
    };

  let fmap = (fn: 'a => 'b, result: tri('a)): tri('b) =>
    switch (result) {
    | Success(response) => Success(fn(response))
    | Error(e) => Error(e)
    | Loading => Loading
    | RelevantDataMissing => RelevantDataMissing
    };

  let optionalToMissing = (result: tri(option('a))): tri('b) =>
    switch (result) {
    | Success(Some(response)) => Success(response)
    | Success(None) => RelevantDataMissing
    | Error(e) => Error(e)
    | Loading => Loading
    | RelevantDataMissing => RelevantDataMissing
    };
};

let apolloResponseToResult2 = (result: ReasonApolloTypes.queryResponse('a)) =>
  result |> QResponse.fromApollo;