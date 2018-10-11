[@bs.val] external environment : string = "process.env.ENV";
Js.log(environment);

let auth0Domain =
  switch (environment) {
  | "DEVELOPMENT" => "guesstimate.auth0.com"
  | "PRODUCTION" => "foretold.auth0.com"
  };

let auth0ClientId =
  switch (environment) {
  | "DEVELOPMENT" => "WYfDFWDK5l9dRq13gtKhXs82AjwbRnm5"
  | "PRODUCTION" => "UjTAICaqODLbSXjmgClkSYbasgll7To8"
  };

let redirectUrl =
  switch (environment) {
  | "DEVELOPMENT" => "http://localhost:1234/callback"
  | "PRODUCTION" => "http://foretold.io/callback"
  };

let serverUrl =
  switch (environment) {
  | "DEVELOPMENT" => "http://localhost:4000/graphql"
  | "PRODUCTION" => "https://prediction-backend.herokuapp.com/graphql"
  };