open Utils;
open Foretold__GraphQL;

let make' = (loggedInUser, measurables) =>
  <C.Measurables.BasicTable measurables loggedInUser showExtraData=true />;

let component = ReasonReact.statelessComponent("Measurables");
let make = (~id: string, ~loggedInUser: Context.Primary.User.t, _children) => {
  ...component,
  render: _self =>
    make'(loggedInUser) |> Queries.Measurables.componentWithCreator(id),
};