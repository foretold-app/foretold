let component = ReasonReact.statelessComponent("StatSummary");

module Styles = {
  open Css;
  let textOverlay = style([position(`absolute)]);
  let mainText = style([fontSize(`em(1.1)), color(FC__Colors.darkLink)]);
  let secondaryText =
    style([fontSize(`em(0.9)), color(FC__Colors.accentBlue)]);
};

let make = (~cdf: FC__Types.Dist.t, _children) => {
  ...component,
  render: _ =>
    <>
      <div className=Styles.mainText>
        <FC__NumberShower
          precision=3
          number={cdf |> FC__Types.Dist.findX(0.5)}
        />
      </div>
      <div className=Styles.secondaryText>
        <FC__NumberShower
          precision=3
          number={cdf |> FC__Types.Dist.findX(0.05)}
        />
        {" to " |> ReasonReact.string}
        <FC__NumberShower
          precision=3
          number={cdf |> FC__Types.Dist.findX(0.95)}
        />
      </div>
    </>,
};