[@bs.val] external environment : string = "process.env.ENV";

let auth0Domain =
  switch (environment) {
  | "DEVELOPMENT" => "guesstimate.auth0.com"
  | "PRODUCTION" => "foretold.auth0.com"
  };

let auth0ClientId =
  switch (environment) {
  | "DEVELOPMENT" => "gn1bwgJfK5Y6jfL6x7t6fB43ZAN3eSnT"
  | "PRODUCTION" => "oIh53ohBnA4xqhaO8vZXa7PiW6TCXjMV"
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

Js.log(environment);
Js.log(serverUrl);
Js.log(auth0Domain);
Js.log(auth0ClientId);