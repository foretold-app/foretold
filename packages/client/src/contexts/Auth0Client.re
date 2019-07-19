type logoutType = {
  .
  "clientId": string,
  "returnTo": string,
};

type t = {
  .
  "authorize": [@bs.meth] (unit => unit),
  "logout": [@bs.meth] (logoutType => unit),
};

type clientOptions = {
  .
  "domain": string,
  "clientID": string,
  "redirectUri": string,
  "responseType": string,
  "scope": string,
};

[@bs.module "auth0-js"] [@bs.new]
external createClient: clientOptions => t = "WebAuth";

let authOptions = {
  "domain": Env.auth0Domain,
  "clientID": Env.auth0ClientId,
  "redirectUri": Env.redirectUrl,
  "responseType": "token id_token",
  "scope": "openid email profile",
};

let triggerLoginScreen = () =>
  authOptions |> createClient |> (c => c##authorize());

let logoutOptions: logoutType = {
  "clientId": Env.auth0ClientId,
  "returnTo": Env.logoutUrl,
};

let logout = () =>
  authOptions |> createClient |> (c => c##logout(logoutOptions));