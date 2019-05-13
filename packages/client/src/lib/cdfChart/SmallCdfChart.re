open Utils;

let component = ReasonReact.statelessComponent("SmallCdfChart");

let foo = {
  "xs": [|50., 100., 300., 400., 500., 600.|],
  "ys": [|0.1, 0.4, 0.6, 0.7, 0.8, 0.9|],
};

module Styles = {
  open Css;
  let graph =
    style([
      selector(".axis", [fontSize(`px(9))]),
      selector(".domain", [display(`none)]),
      selector(".tick line", [display(`none)]),
      selector(".tick text", [color(`hex("bfcad4"))]),
      selector(".chart .area-path", [SVG.fill(`hex("7e9db7"))]),
    ]);
};
let make = (~data, ~minX, ~maxX, _children) => {
  ...component,
  render: _ =>
    <div className=Styles.graph>
      <CdfChart
        width=300
        height=50
        minX
        maxX
        marginBottom=15
        showVerticalLine=false
        showDistributionLines=false
        primaryDistribution=data
      />
    </div>,
};