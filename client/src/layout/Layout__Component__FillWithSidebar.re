open Utils;
open Style.Grid;
open Foretold__GraphQL;

module Styles = {
  open Css;
  let outer =
    style([
      display(`flex),
      flexDirection(`row),
      minHeight(`percent(100.0)),
      backgroundColor(`hex("F0F2F5")),
    ]);
  let left =
    style([
      flex(1),
      backgroundColor(`hex("2c436e")),
      minWidth(`px(200)),
    ]);
  let right = style([flex(7), width(`percent(100.0))]);
};

let component = ReasonReact.statelessComponent("SidebarFill");
let make =
    (
      ~channelId: option(string),
      ~loggedInUser: Context.Primary.User.t,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className=Styles.outer>
      <div className=Styles.left>
        <Layout__Component__Sidebar channelId loggedInUser />
      </div>
      <div className=Styles.right>
        <Layout__Component__Header loggedInUser />
        <div> ..._children </div>
      </div>
    </div>,
};