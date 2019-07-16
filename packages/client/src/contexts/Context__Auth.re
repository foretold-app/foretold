open Rationale.Function.Infix;
type error = string;

module CallbackUrlToAuth0Tokens = {
  open Utils;

  let matchAccessToken = [%re "/access_token=([^\$&]+)/g"];
  let matchIdToken = [%re "/id_token=([^\$&]+)/g"];
  let matchToken = [%re "/token=([^\$&]+)/g"];

  let make = (url: ReasonReact.Router.url) => {
    let accessToken = url.hash |> resolveRegex(matchAccessToken);
    let idToken = url.hash |> resolveRegex(matchIdToken);
    let token = url.hash |> resolveRegex(matchToken);

    let monthMs = 31.  *.  ( 24. *. 60.0 *. 60. *. 1000. );
    let currentTimeMs = E.JsDate.make() |> E.JsDate.valueOf;
    let expiresAtInMs = currentTimeMs +. monthMs;
    let expiresAt = expiresAtInMs |> Int64.of_float |> Int64.to_string;

    switch (accessToken, idToken) {
    | (_, "") => None
    | ("", _) => None
    | _ => Some(Auth0Tokens.make(accessToken, idToken, expiresAt, token))
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

  let logoutIfTokenIsObsolete = (tokens: Auth0Tokens.t) =>
    if (tokens |> Auth0Tokens.isObsolete) {
      logout();
    };
};