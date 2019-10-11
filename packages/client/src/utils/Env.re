[@bs.val] external serverUrl: string = "window.ENV.API_URL";
[@bs.val] external auth0Domain: string = "window.ENV.AUTH0_DOMAIN";
[@bs.val] external auth0ClientId: string = "window.ENV.AUTH0_CLIENT_ID";
[@bs.val] external mode: string = "window.ENV.MODE";
[@bs.val] external intercomAppId: string = "window.ENV.INTERCOM_APP_ID";

[@bs.val] external origin: string = "window.location.origin";

let redirectUrl = origin ++ "/callback";
let logoutUrl = origin ++ "/login";
let prod = mode == "production";
let dev = mode == "development";

Js.log("---");
Js.log2("API URL:", serverUrl);
Js.log2("Auth0 Domain:", auth0Domain);
Js.log2("Auth0 Client Id:", auth0ClientId);
Js.log2("Redirect URL:", redirectUrl);
Js.log2("Mode:", mode);
Js.log2("Intercom App Id:", intercomAppId);
Js.log("---");
