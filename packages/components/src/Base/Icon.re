[@bs.config {jsx: 3}];

module Questionmark = {
  // Adapted to pagecard title
  let circle =
    Css.(
      style([
        width(`em(1.0)),
        height(`em(1.0)),
        borderRadius(`percent(50.)),
        background(`hex("aaa")),
      ])
    );

  let insideStyle =
    Css.(
      style([
        color(`hex("333")),
        cursor(`pointer),
        fontSize(`em(0.9)),
        marginLeft(`em(-0.18)),
      ])
    );
  // Local icon style
  let questionMarkstyle = isInteractive =>
    Css.(
      style([
        width(`em(1.0)),
        height(`em(1.0)),
        textAlign(`center),
        borderRadius(`percent(50.)),
        display(`inlineBlock),
        background(`hex("aaa")),
        fontStyle(`italic),
        lineHeight(`em(0.85)),
        fontSize(`em(1.0)),
        opacity(0.5),
        fontWeight(`num(600)),
        cursor(`pointer),
        hover(isInteractive ? [opacity(1.0)] : []),
      ])
    );

  [@react.component]
  let make = (~isInteractive=true) =>
    <div className={questionMarkstyle(isInteractive)}>
      <span className=insideStyle> {React.string("i")} </span>
    </div>;
};

module DownArrow = {
  /* Down array from ant */
  let buttonStyle = Css.(style([marginLeft(`px(8))]));

  [@react.component]
  let make = () =>
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
};
