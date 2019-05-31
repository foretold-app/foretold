let component = ReasonReact.statelessComponent("SmallCdfChart");

module Styles = {
  open Css;
  let textOverlay = style([position(`absolute)]);
  let mainText = style([fontSize(`em(1.1)), color(FC__Colors.darkLink)]);
  let secondaryText =
    style([fontSize(`em(0.9)), color(FC__Colors.accentBlue)]);

  let graph = chartColor =>
    style([
      position(`relative),
      selector(".axis", [fontSize(`px(9))]),
      selector(".domain", [display(`none)]),
      selector(".tick line", [display(`none)]),
      selector(".tick text", [color(`hex("bfcad4"))]),
      selector(".chart .area-path", [SVG.fill(chartColor)]),
    ]);
};

let make =
    (~cdf: FC__Types.Dist.t, ~minX, ~maxX, ~color=`hex("7e9db7"), _children) => {
  ...component,
  render: _ => {
    let pdf = cdf |> FC__Types.Dist.toPdf;

    <div className={Styles.graph(color)}>
      <div className=Styles.textOverlay>
        <FC__CdfChart__StatSummary cdf />
      </div>
      <FC__CdfChart__Base
        width=200
        height=40
        minX
        maxX
        marginBottom=0
        marginTop=0
        showVerticalLine=false
        showDistributionLines=false
        primaryDistribution={"xs": pdf.xs, "ys": pdf.ys}
        onHover={_r => ()}
      />
    </div>;
  },
};