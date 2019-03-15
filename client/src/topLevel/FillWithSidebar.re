open Utils;
open Style.Grid;

module Styles = {
  open Css;
  let outer =
    style([
      display(`flex),
      flexDirection(`row),
      minHeight(`percent(100.0)),
    ]);
  let left =
    style([
      flex(1),
      backgroundColor(`hex("2c436e")),
      minWidth(`px(200)),
    ]);
  let right = style([flex(7), width(`percent(100.0))]);
  let rightTop =
    style([
      borderBottom(`px(1), `solid, `hex("eee")),
      paddingLeft(`px(10)),
      paddingBottom(`em(0.8)),
      paddingRight(`em(0.4)),
      paddingTop(`px(10)),
    ]);
  let rightBottom =
    style([
      paddingLeft(`px(10)),
      paddingRight(`px(10)),
      paddingTop(`px(10)),
    ]);
  let header =
    style([color(`hex("333")), fontSize(`em(2.0)), fontWeight(`bold)]);
  let newButton =
    style([
      color(`hex("fff")),
      borderRadius(`px(4)),
      padding2(~v=`px(1), ~h=`em(0.3)),
      Css.cursor(`pointer),
      float(`right),
      fontSize(`em(1.2)),
      marginTop(`em(0.5)),
      backgroundColor(hex("1890ff")),
      selector(
        ":hover",
        [backgroundColor(`hex("60b2ff")), color(`hex("fff"))],
      ),
    ]);
};

let component = ReasonReact.statelessComponent("SidebarFill");
let make = (~channel: option(string), ~loggedInUser: GetUser.t, children) => {
  ...component,
  render: _self =>
    <UseRouterForLinks>
      <div className=Styles.outer>
        <div className=Styles.left> <Sidebar channel loggedInUser /> </div>
        <div className=Styles.right> children </div>
      </div>
    </UseRouterForLinks>,
};