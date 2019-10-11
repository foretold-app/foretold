type logoutType = {
  .
  "clientId": string,
  "returnTo": string,
};

type authResult = {
  .
  "accessToken": string,
  "expiresIn": string,
  "idToken": string,
};

type authErr = {. "message": string};

type checkSessionOptions = {.};

type t = {
  .
  "authorize": [@bs.meth] (unit => unit),
  "checkSession":
    [@bs.meth] ((checkSessionOptions, (authErr, authResult) => unit) => unit),
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

let checkSessionOptions = {};

let client = authOptions |> createClient;

let triggerLoginScreen = () => client##authorize();

let checkSession = () => {
  client##checkSession(
    checkSessionOptions,
    (authErr, authResult) => {
      Js.log2("authErr", authErr);
      Js.log2("authResult", authResult);
      ();
    },
  );

  ();
};

let logoutOptions: logoutType = {
  "clientId": Env.auth0ClientId,
  "returnTo": Env.logoutUrl,
};

let logout = () => client##logout(logoutOptions);
