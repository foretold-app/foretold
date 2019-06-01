
module Questionmark = {
  let component = ReasonReact.statelessComponent(__MODULE__);
  // Adapted to pagecard title
  let iconStyle =
    Css.(
      style([
        height(`px(21)),
        width(`px(21)),
        paddingLeft(`em(0.6)),
        verticalAlign(`bottom),
        margin2(~v=`px(-2), ~h=`zero),
        cursor(`default),
      ])
    );
  // Local icon style
  let questionMarkstyle =
    Css.(
      style([
        selector("text", [fontSize(`em(0.7)), SVG.fill(FC__Colors.textMedium)]),
        selector("circle", [SVG.stroke(FC__Colors.greyO4)]),
      ])
    );
  let make = _children => {
    ...component,
    render: _self => {
      <svg
        className={Css.merge([iconStyle, questionMarkstyle])}
        viewBox="0 0 20 20">
        <text x="10" y="13" fontWeight="bold" textAnchor="middle">
          {React.string("?")}
        </text>
        <circle cx="10" cy="10" r="8.5" strokeWidth="1" fill="none" />
      </svg>;
    },
  };
};
