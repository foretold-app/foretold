let component = ReasonReact.statelessComponent(__MODULE__);

let inputHeader =
  Css.(
    style([
      fontWeight(`bold),
      paddingLeft(`em(0.4)),
      paddingBottom(`em(0.6)),
    ])
  );

let make = children => {
  ...component,
  render: _self => <div className=inputHeader> ...children </div>,
};
