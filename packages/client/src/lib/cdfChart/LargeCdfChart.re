[@bs.config {jsx: 3}];

open Utils;

module Styles = {
  open Css;
  let graph =
    style([
      selector(".axis", [fontSize(`px(11))]),
      selector(".domain", [display(`none)]),
      selector(".tick line", [display(`none)]),
      selector(".tick text", [color(`hex("2e4c65"))]),
      selector(".chart .area-path", [SVG.fill(`hex("7e9db7"))]),
    ]);
};

[@react.component]
let make = (~data) =>
  <div className=Styles.graph>
    <CdfChart
      marginBottom=25
      width=400
      height=200
      showVerticalLine=false
      showDistributionLines=false
      primaryDistribution=data
    />
  </div>;
