let component = ReasonReact.statelessComponent(__MODULE__);

let make = (~fullWidth=false, ~rows=5, ~value=?, _children) => {
  ...component,
  render: _self =>
    <textarea className={FC__FormStyles.widthStyle(~fullWidth, ())} rows>
      {value |> FC__E.O.React.fmapOrNull(React.string)}
    </textarea>,
};