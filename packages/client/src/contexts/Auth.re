module UrlToAuth0Tokens = {
  open Utils;

  // Always use fresh RegExp objects
  let matchAccessToken = () => [%re "/access_token=([^\$&]+)/g"];
  let matchIdToken = () => [%re "/id_token=([^\$&]+)/g"];

  let expiresAt = {
      let monthMs = 31.  *.  ( 24. *. 60.0 *. 60. *. 1000. );
      let currentTimeMs = E.JsDate.make() |> E.JsDate.valueOf;
      let expiresAtInMs = currentTimeMs +. monthMs;

      expiresAtInMs |> Int64.of_float |> Int64.to_string
  };

  let make = (url: ReasonReact.Router.url) => {
    let accessToken = url.hash |> resolveRegex(matchAccessToken());
    let idToken = url.hash |> resolveRegex(matchIdToken());

    switch (accessToken, idToken) {
    | ("", _) => None
    | (_, "") => None
    | _ => Some(Auth0Tokens.make(accessToken, idToken, expiresAt))
    }
  };
};

module UrlToTokens = {
  let make = (url: ReasonReact.Router.url): option(string) => {
    let keyValuePairs = url.hash |> KeyValuePairs.fromSearchParam;

    switch (keyValuePairs |> KeyValuePairs.get("token")) {
      | Some(s) => Some(s)
      | _ => None
    };
  };
};

module Actions = {
  let logout = () => {
    Js.log("Logout");
    Auth0Tokens.destroy();
    ServerJwt.destroy();
    Auth0Client.logout();
    ReasonReact.Router.push("/login");
    ();
  };
};