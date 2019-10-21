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
      backgroundColor(`hex("283c5b")),
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

let component = ReasonReact.statelessComponent("FillWithSidebar");
let make = (~channelId=None, ~loggedUser: option(Types.user), _children) => {
  ...component,
  render: _self =>
    <div className=Styles.outer>
      <div className=Styles.left> <Sidebar channelId loggedUser /> </div>
      <div className=Styles.right>
        <div className=Styles.rightInner>
          <Header loggedUser />
          <div> ..._children </div>
        </div>
        <Footer />
      </div>
    </div>,
};