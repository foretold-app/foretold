module BorderedBox = {
  let component = ReasonReact.statelessComponent("BorderedBox");
  module Styles = {
    open Css;
    let item =
      style([
        maxWidth(px(1500)),
        display(`flex),
        flexDirection(`row),
        padding(px(12)),
        borderRadius(px(2)),
        border(px(1), `solid, `hex("eee")),
        marginBottom(px(20)),
      ]);
  };

  let make = children => {
    ...component,
    render: _ => <div className=Styles.item> ...children </div>,
  };
};