let component = ReasonReact.statelessComponent("AgentLayoutPage");
let make = ({head, body}: SLayout.LayoutConfig.t) => {
  ...component,
  render: _ => {
    <FC.PageCard>
      <FC.PageCard.HeaderRow> head </FC.PageCard.HeaderRow>
      body
    </FC.PageCard>;
  },
};

let makeWithEl = (t: SLayout.LayoutConfig.t) => make(t) |> E.React.el;