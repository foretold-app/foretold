[@bs.config {jsx: 3}];

let valueFromEvent = (evt): string => evt->ReactEvent.Form.target##value;

module Styles = {
  open Css;

  let input = style([height(`em(10.)), width(`percent(100.0))]);
};

let default = {|
{
    "columns": [
        {"id":"name", "name": "Name", "columnType": "String"},
        {"id":"metric1", "name": "Metric 1", "columnType": "MeasurableId"}
    ],
    "data": [
        {"name": "Thing1", "metric1": "sdf98sdjf8sjdf8j"},
        {"name": "Thing2", "metric1": "sdf98sdjf8sjdf8j"}
    ]
}
|};

[@react.component]
let make = (~channelId: string) => {
  let (text, setText) = React.useReducer((oldText, newText) => newText, "");
  <div>
    <textarea
      value=text
      className=Styles.input
      onChange={event => setText(valueFromEvent(event))}
    />
    {switch (Json.parse(text)) {
     | Some(json) => <DashboardTableC channelId tableJson=json />
     | None => "sdf" |> Utils.ste
     }}
  </div>;
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("ChannelTable");
  let make = (~channelId, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~channelId, ()),
      children,
    );
};