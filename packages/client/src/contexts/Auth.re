type error = string;

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
  open Utils;

  let matchToken = () => [%re "/authToken=([^\$&]+)/g"];

  let make = (url: ReasonReact.Router.url) => {
    let token = url.hash |> resolveRegex(matchToken());

    switch (token) {
    | "" => None
    | _ => Some(Tokens.make(token))
    }
  };
};

module Actions = {
  let logout = () => {
    Auth0Tokens.destroy();
    ServerJwt.destroy();
    Auth0Client.logout();
    ReasonReact.Router.push("/");
    ();
  };

  let logoutIfTokenIsObsolete = (tokens: Auth0Tokens.t): bool =>
    if (tokens |> Auth0Tokens.isObsolete) {
      logout();
      true
    } else {
      false
    }
};