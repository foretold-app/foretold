[@bs.config {jsx: 3}];

[@react.component]
let make = (~fullWidth=false, ~rows=5, ~value=?) =>
  <textarea className={FormStyles.widthStyle(~fullWidth, ())} rows>
    {value |> E.O.React.fmapOrNull(React.string)}
  </textarea>;
