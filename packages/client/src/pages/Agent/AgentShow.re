open Utils;
open Foretold__GraphQL;
open Css;

let block = style([marginBottom(`em(1.0))]);
let notFound = "Agent not found" |> ste |> E.React.inH3;

/* TODO: Improve this bit */
let agentSection = (e: Queries.Agent.agent) =>
  switch (e) {
  | {bot: Some(r)} =>
    <>
      {r.name |> ste |> E.React.inH2}
      {r.description
       |> E.O.fmap(r => r |> ste |> E.React.inH3)
       |> E.O.React.defaultNull}
      {r.competitorType
       |> Context.Primary.CompetitorType.toString
       |> ste
       |> E.React.inH3}
    </>
  | {user: Some(r)} => r.name |> ste |> E.React.inH1
  | _ => notFound
  };

let component = ReasonReact.statelessComponent("AgentShow");

type pageParams = {id: string};
let make = (~pageParams, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    Queries.Agent.component(~id=pageParams.id, ({measurementsList}) =>
      SLayout.LayoutConfig.make(
        ~head="" |> ste,
        ~body=measurementsList |> C.Measurements.Table.make2,
      )
      |> layout
    ),
};