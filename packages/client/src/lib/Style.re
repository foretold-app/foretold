[@bs.config {jsx: 3}];

module BorderedBox = {
  module Styles = {
    open Css;
    let item =
      style([
        display(`flex),
        flexDirection(`row),
        padding(px(12)),
        borderRadius(px(2)),
        border(px(1), `solid, `hex("eee")),
        marginBottom(px(20)),
      ]);
  };

  [@react.component]
  let make = (~children=<Null />) =>
    <div className=Styles.item> children </div>;
};

module Grid = {
  open Css;

  module Div = {
    [@react.component]
    let make =
        (~styles=[], ~flex=?, ~flexDirection=?, ~float=?, ~children=<Null />) => {
      let flexStyle =
        flex |> E.O.fmap(e => Css.style([Css.flex(e)])) |> E.O.default("");

      let floatStyle =
        float |> E.O.fmap(e => Css.style([Css.float(e)])) |> E.O.default("");

      let directionStyle =
        flexDirection
        |> E.O.fmap(e => Css.style([display(`flex), Css.flexDirection(e)]))
        |> E.O.default("");

      let allStyles = [flexStyle, directionStyle, floatStyle, ...styles];

      <div className={E.L.join(" ", allStyles)}> children </div>;
    };
  };
};