module Questionmark = {
  let component =
    ReasonReact.statelessComponent(__MODULE__ ++ " QuestionMark");
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
        selector(
          "text",
          [fontSize(`em(0.7)), SVG.fill(FC__Settings.textMedium)],
        ),
        selector("circle", [SVG.stroke(FC__Settings.greyO4)]),
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

module DownArrow = {
  /* Down array from ant */
  let component = ReasonReact.statelessComponent(__MODULE__ ++ " DownArrow");
  let buttonStyle = Css.(style([marginLeft(`px(8))]));
  let make = _children => {
    ...component,
    render: _self => {
      <svg
        className=buttonStyle
        viewBox="64 64 896 896"
        width="0.8em"
        height="0.8em"
        fill="currentColor"
        ariaHidden=true
        focusable="false">
        <path
          d="M884 256h-75c-5.1 0-9.9 2.5-12.9 6.6L512 654.2 227.9 262.6c-3-4.1-7.8-6.6-12.9-6.6h-75c-6.5 0-10.3 7.4-6.5 12.7l352.6 486.1c12.8 17.6 39 17.6 51.7 0l352.6-486.1c3.9-5.3.1-12.7-6.4-12.7z"
        />
      </svg>;
    },
  };
};