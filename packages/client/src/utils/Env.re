/* [@bs.val] external environment: string = "DEVELOPMENT"; */
[@bs.val] external apiUrlEnv: string = "process.env.API_URL";
[@bs.val] external auth0DomainEnv: string = "process.env.AUTH0_DOMAIN";
[@bs.val] external auth0ClientIdEnv: string = "process.env.AUTH0_CLIENT_ID";
[@bs.val] external origin: string = "window.location.origin";

let auth0Domain = auth0DomainEnv;
let auth0ClientId = auth0ClientIdEnv;
let redirectUrl = origin ++ "/callback";
let logoutUrl = origin ++ "/";
let serverUrl = apiUrlEnv;

Js.log("---");
Js.log2("API URL:", serverUrl);
Js.log2("Auth0 Domain:", auth0Domain);
Js.log2("Auth0 Client Id:", auth0ClientId);
Js.log2("Redirect URL:", redirectUrl);
Js.log("---");