open Belt;
open Rationale.Option;
open Utils;

type generatedAuth0Client = {. "authorize": [@bs.meth] (unit => unit)};

type clientOptions = {
  .
  "domain": string,
  "clientID": string,
  "redirectUri": string,
  "responseType": string,
  "scope": string,
};

[@bs.module "auth0-js"] [@bs.new]
external createClient: clientOptions => generatedAuth0Client = "WebAuth";

[@bs.module] external jwt_decode: string => Js.Json.t = "jwt-decode";

let safe_jwt_decode = (s: string): option('a) =>
  try (Some(jwt_decode(s))) {
  | _ => None
  };

let getAuth0Id = (idToken: string): option(string) =>
  idToken
  |> safe_jwt_decode
  >>= Js.Json.decodeObject
  >>= (e => Js.Dict.get(e, "sub"))
  >>= Js.Json.decodeString;

let matchAccessToken = [%re "/access_token=([^\$&]+)/g"];
let matchExpiresIn = [%re "/expires_in=([^\$&]+)/g"];
let matchIdToken = [%re "/id_token=([^\$&]+)/g"];

let handleAuth = (url: ReasonReact.Router.url) => {
  let accessToken = url.hash |> resolveRegex(matchAccessToken);
  let idToken = url.hash |> resolveRegex(matchIdToken);
  let expiresIn = url.hash |> resolveRegex(matchExpiresIn);
  let addedMs = expiresIn |> float_of_string |> (e => e *. 1000.);
  let currentTimeMs = Js.Date.make() |> Js.Date.valueOf;
  let expiresAtInMs = currentTimeMs +. addedMs;
  let expiresAt = expiresAtInMs |> Int64.of_float |> Int64.to_string;
  open Dom.Storage;
  localStorage |> setItem("access_token", accessToken);
  localStorage |> setItem("id_token", idToken);
  localStorage |> setItem("expires_at", expiresAt);
  ();
};

let logout = () => {
  open Dom.Storage;
  localStorage |> removeItem("access_token");
  localStorage |> removeItem("id_token");
  localStorage |> removeItem("expires_at");
  ReasonReact.Router.push("/");
  ();
};

let expiresAt = () => Dom.Storage.(localStorage |> getItem("expires_at"));

let authIsObsolete = () => {
  let exp = expiresAt();
  exp
  |> E.Option.fmap(Int64.of_string)
  |> E.Option.fmap(Int64.to_float)
  |> E.Option.fmap(e => e < Js.Date.now())
  |> E.Option.dimap(idd, () => false);
};

let authToken = () => Dom.Storage.(localStorage |> getItem("id_token"));

let logoutIfTokenIsObsolete = () =>
  if (authIsObsolete()) {
    logout();
  };

let isLoggedIn = () => {
  logoutIfTokenIsObsolete();
  Dom.Storage.(localStorage |> getItem("id_token"))
  |> (
    e =>
      switch (e) {
      | Some(r) => r != ""
      | None => false
      }
  );
};

let getIdToken = () => Dom.Storage.(localStorage |> getItem("id_token"));

let authOptions = {
  "domain": Env.auth0Domain,
  "clientID": Env.auth0ClientId,
  "redirectUri": Env.redirectUrl,
  "responseType": "token id_token",
  "scope": "openid",
};

let authClient = createClient(authOptions);

let logIn = () => authClient##authorize();
let userId = () => Rationale.Option.bind(getIdToken(), getAuth0Id);