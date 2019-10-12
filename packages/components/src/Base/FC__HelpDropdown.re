let component = ReasonReact.statelessComponent("HelpDropdown");

let staticOverlay =
  <div
    className=Css.(
      style([
        border(`px(1), `solid, FC__Settings.grey1),
        backgroundColor(FC__Settings.white),
        width(`px(200)),
      ])
    )>
    {"Yo Dog!!!!" |> React.string}
  </div>;

let make = _children => {
  ...component,
  render: _self => {
    <FC__Dropdown overlay=staticOverlay trigger=FC__Dropdown.Hover>
      <span> <FC__Icon.Questionmark /> </span>
    </FC__Dropdown>;
  },
};