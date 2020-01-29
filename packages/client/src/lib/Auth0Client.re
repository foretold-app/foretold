type logoutType = {
  .
  "clientId": string,
  "returnTo": string,
};

type authResult = {
  .
  "accessToken": string,
  "expiresIn": int,
  "idToken": string,
};

type authErr = Js.Nullable.t({. "message": string});

type t = {
  .
  "authorize": [@bs.meth] (unit => unit),
  "checkSession":
    [@bs.meth] ((Js.Dict.t(string), (authErr, authResult) => unit) => unit),
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

let client = authOptions |> createClient;

let triggerLoginScreen = () => client##authorize();

let checkSession = fn => {
  client##checkSession(
    Js.Dict.empty(),
    (authErr, authResult) => {
      switch (authErr |> Js.Nullable.toOption) {
      | Some(err) => Js.log2("Check session error:", err)
      | None => authResult |> fn
      };
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