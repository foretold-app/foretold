/* [@bs.val] external environment: string = "DEVELOPMENT"; */
[@bs.val] external environment: string = "process.env.ENV";
[@bs.val] external apiUrl: string = "process.env.API_URL";

let auth0Domain =
  switch (environment) {
  | "DEVELOPMENT" => "guesstimate.auth0.com"
  | "STAGING" => "guesstimate.auth0.com"
  | "PRODUCTION" => "foretold.auth0.com"
  };

let auth0ClientId =
  switch (environment) {
  | "DEVELOPMENT" => "gn1bwgJfK5Y6jfL6x7t6fB43ZAN3eSnT"
  | "STAGING" => "gn1bwgJfK5Y6jfL6x7t6fB43ZAN3eSnT"
  | "PRODUCTION" => "oIh53ohBnA4xqhaO8vZXa7PiW6TCXjMV"
  };

let redirectUrl =
  switch (environment) {
  | "DEVELOPMENT" =>
    %raw
    "window.location.origin + '/callback'"
  | "STAGING" =>
    %raw
    "window.location.origin + '/callback'"
  | "PRODUCTION" =>
    %raw
    "window.location.origin + '/callback'"
  };

let serverUrl =
  switch (environment) {
  | "DEVELOPMENT" => apiUrl
  | "STAGING" => apiUrl
  | "PRODUCTION" => apiUrl
  };

Js.log(environment);
Js.log(serverUrl);
Js.log(auth0Domain);
Js.log(auth0ClientId);
Js.log(redirectUrl);
