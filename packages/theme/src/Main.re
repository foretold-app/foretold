let foo = "working!" |> ReasonReact.string;

module BaseLink = {
  let component = ReasonReact.statelessComponent(__MODULE__);

  let defaultColors = (Css_Colors.yellow, Css_Colors.tomato);

  let headerLink = (~colors, ~styles) => {
    let (mainColor, hoverColor) = colors;

    Css.(
      style(
        [
          cursor(`pointer),
          color(mainColor),
          textDecoration(`none),
          selector(":hover", [color(hoverColor)]),
        ]
        @ styles,
      )
    );
  };

  let make =
      (~href="#", ~onClick=?, ~colors=defaultColors, ~styles=[], children) => {
    ...component,
    render: _self =>
      <a href className={headerLink(~colors, ~styles)} ?onClick>
        ...children
      </a>,
  };
};

module Header = {};