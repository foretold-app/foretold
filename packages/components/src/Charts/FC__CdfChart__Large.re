let component = ReasonReact.statelessComponent("SmallCdfChart");

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
let make = (~cdf: FC__Types.Dist.t, ~width=Some(400), _children) => {
  ...component,
  render: _ => {
    Js.log2("HIHI", cdf);
    let pdf = cdf |> FC__Types.Dist.toPdf;
    <div className=Styles.graph>
      <FC__CdfChart__Base
        marginBottom=25
        width=?width
        height=200
        showVerticalLine=false
        showDistributionLines=false
        primaryDistribution={"xs": pdf.xs, "ys": pdf.ys}
        onHover={_r => ()}
      />
    </div>
  },
};