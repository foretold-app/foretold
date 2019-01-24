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

module Grid = {
  open Css;
  module Styles = {
    let flexRow = style([display(`flex), flexDirection(`row)]);
    let flexColumn = style([display(`flex), flexDirection(`column)]);
    let flex = n => style([flex(n)]);
  };

  module Div = {
    let component = ReasonReact.statelessComponent("divWithStyles");
    let make = (~styles=[], children) => {
      ...component,
      render: _ =>
        <div className={Rationale.RList.join(" ", styles)}>
          ...children
        </div>,
    };
  };
};