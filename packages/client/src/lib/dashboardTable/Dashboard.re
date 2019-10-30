let valueFromEvent = (evt): string => evt->ReactEvent.Form.target##value;

module Styles = {
  open Css;

  let input = style([height(`em(10.)), width(`percent(100.0))]);
  let padding = style([padding(`em(1.0))]);
};

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
    <SLayout isFluid=true head=ReasonReact.null>
      <FC.PageCard.Body>
        <div className=Styles.padding>
          <Antd.Input.TextArea
            style={ReactDOMRe.Style.make(~minHeight="6em", ())}
            onChange={event => self.send(Update(valueFromEvent(event)))}
            value={self.state.text}
          />
        </div>
        <Markdown source={self.state.text} channelId supportForetoldJs=true />
      </FC.PageCard.Body>
    </SLayout>;
  },
};