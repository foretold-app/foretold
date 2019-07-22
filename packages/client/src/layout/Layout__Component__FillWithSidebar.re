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
      flex(`num(1.)),
      backgroundColor(`hex("2c436e")),
      minWidth(`px(200)),
    ]);
  let right =
    style([
      flex(`num(7.)),
      width(`percent(100.0)),
      display(`flex),
      minHeight(`vh(100.0)),
      flexDirection(`column),
    ]);
  let rightInner = style([flex(`num(1.))]);
};

let component = ReasonReact.statelessComponent("SidebarFill");
let make =
    (~channelId: option(string), ~loggedInUser: Primary.User.t, _children) => {
  ...component,
  render: _self =>
    <div className=Styles.outer>
      <div className=Styles.left>
        <Layout__Component__Sidebar channelId loggedInUser />
      </div>
      <div className=Styles.right>
        <div className=Styles.rightInner>
          <Layout__Component__Header loggedInUser />
          <div> ..._children </div>
        </div>
        <Footer />
      </div>
    </div>,
};