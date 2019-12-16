module Styles = {
  open Css;
  let graph = chartColor =>
    style([
      selector(".axis", [fontSize(`px(9))]),
      selector(".domain", [display(`none)]),
      selector(".tick line", [display(`none)]),
      selector(".tick text", [color(`hex("bfcad4"))]),
      selector(".chart .area-path", [SVG.fill(chartColor)]),
    ]);
};

[@react.component]
let make =
    (
      ~data,
      ~minX=None,
      ~maxX=None,
      ~width=300,
      ~height=50,
      ~color=`hex("7e9db7"),
    ) =>
  <div className={Styles.graph(color)}>
    <FC__CdfChart__Base
      width
      height
      ?minX
      ?maxX
      marginBottom=15
      showVerticalLine=false
      showDistributionLines=false
      primaryDistribution=data
    />
  </div>;