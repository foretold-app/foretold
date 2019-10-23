[@react.component]
let make = (~fullWidth=false, ~rows=5, ~value=?) =>
  <textarea className={FC__FormStyles.widthStyle(~fullWidth, ())} rows>
    {value |> FC__E.O.React.fmapOrNull(React.string)}
  </textarea>;

module Jsx2 = {
  let component = ReasonReact.statelessComponent(__MODULE__ ++ "Jsx2");

  let make = (~fullWidth=false, ~rows=5, ~value=?, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~fullWidth, ~rows, ~value?, ()),
      children,
    );
};
