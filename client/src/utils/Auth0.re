open Belt;
open Rationale.Option;

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
external createClient : clientOptions => generatedAuth0Client = "WebAuth";

[@bs.module] external jwt_decode : string => Js.Json.t = "jwt-decode";

let safe_jwt_decode = (s: string) : option('a) =>
  try (Some(jwt_decode(s))) {
  | _ => None
  };

let getSub = (idToken: string) : option(string) =>
  idToken
  |> safe_jwt_decode
  >>= Js.Json.decodeObject
  >>= (e => Js.Dict.get(e, "sub"))
  >>= Js.Json.decodeString;

let matchAccessToken = [%re "/access_token=([^\$&]+)/g"];
let matchExpiresIn = [%re "/expires_in=([^\$&]+)/g"];
let matchIdToken = [%re "/id_token=([^\$&]+)/g"];

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

let handleAuth = (url: ReasonReact.Router.url) => {
  let accessToken = url.hash |> resolveRegex(matchAccessToken);
  let idToken = url.hash |> resolveRegex(matchIdToken);
  let expiresIn = url.hash |> resolveRegex(matchExpiresIn);
  open Dom.Storage;
  localStorage |> setItem("access_token", accessToken);
  localStorage |> setItem("id_token", idToken);
  localStorage |> setItem("expires_at", expiresIn);
  ();
};

let logout = () => {
  open Dom.Storage;
  localStorage |> removeItem("access_token");
  localStorage |> removeItem("id_token");
  localStorage |> removeItem("expires_at");
  ();
};

let authToken = () => Dom.Storage.(localStorage |> getItem("id_token"));

let isLoggedIn = () =>
  Dom.Storage.(localStorage |> getItem("id_token")) |> Rationale.Option.isSome;

let getIdToken = () => Dom.Storage.(localStorage |> getItem("id_token"));

let authOptions = {
  "domain": "guesstimate.auth0.com",
  "clientID": "gn1bwgJfK5Y6jfL6x7t6fB43ZAN3eSnT",
  "redirectUri": "http://localhost:1234/callback",
  "responseType": "token id_token",
  "scope": "openid",
};

let authClient = createClient(authOptions);

let logIn = () => authClient##authorize();
let userId = Rationale.Option.bind(getIdToken(), getSub);