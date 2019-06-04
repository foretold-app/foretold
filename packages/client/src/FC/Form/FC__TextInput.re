let component = ReasonReact.statelessComponent(__MODULE__);

let make = (~fullWidth=false, ~placeholder=?, _children) => {
  ...component,
  render: _self =>
    <input
      type_="text"
      className={FC__FormStyles.widthStyle(~fullWidth, ())}
      ?placeholder
    />,
};