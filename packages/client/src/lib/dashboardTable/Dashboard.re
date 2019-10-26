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

type state = {text: string};

type action =
  | Update(string);

let component = ReasonReact.reducerComponent("MeasurableBottomSection");

let make = (~channelId: string, _children) => {
  ...component,
  reducer: (action, _state) =>
    switch (action) {
    | Update(text) => ReasonReact.Update({text: text})
    },

  initialState: () => {text: ""},

  render: self => {
    <div>
      <textarea
        value={self.state.text}
        className=Styles.input
        onChange={event => self.send(Update(valueFromEvent(event)))}
      />
      {switch (Json.parse(self.state.text)) {
       | Some(json) => <DashboardTableC channelId tableJson=json />
       | None => "sdf" |> Utils.ste
       }}
    </div>;
  },
};