let component = ReasonReact.statelessComponent("AgentLayoutPage");
let make = ({head, body}: SLayout.LayoutConfig.t) => {
  ...component,
  render: _ => {
    <SLayout head> body </SLayout>;
  },
};

let makeWithEl = (t: SLayout.LayoutConfig.t) => make(t) |> E.React.el;