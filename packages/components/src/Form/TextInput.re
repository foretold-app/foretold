[@bs.config {jsx: 3}];

[@react.component]
let make = (~fullWidth=false, ~placeholder=?) =>
  <input
    type_="text"
    className={FormStyles.widthStyle(~fullWidth, ())}
    ?placeholder
  />;
