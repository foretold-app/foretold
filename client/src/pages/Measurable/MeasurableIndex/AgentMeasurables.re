open Utils;
open Foretold__GraphQL;

let make' = (loggedInUser, measurables) =>
  <C.Measurables.BasicTable measurables loggedInUser showExtraData=true />;

let component = ReasonReact.statelessComponent("Measurables");
let make =
    (
      ~id: string,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _self =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Agent Measurables"),
      ~body=
        make'(loggedInUser) |> Queries.Measurables.componentWithCreator(id),
    )
    |> layout,
};