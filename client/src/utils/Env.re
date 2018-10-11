[@bs.val] external environment : string = "process.env.ENV";
Js.log(environment);

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