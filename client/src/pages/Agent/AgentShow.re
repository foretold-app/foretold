open Utils;
open Foretold__GraphQL;
open Css;

let block = style([marginBottom(`em(1.0))]);
let notFound = "Agent not found" |> ste |> E.React.inH3;

let agentSection = (e: Queries.Agent.agent) =>
  switch (e) {
  | {bot: Some(r)} =>
    <>
      {
        r.name
        |> E.O.fmap(r => r |> ste |> E.React.inH2)
        |> E.O.React.defaultNull
      }
      {
        r.description
        |> E.O.fmap(r => r |> ste |> E.React.inH3)
        |> E.O.React.defaultNull
      }
      {
        r.competitorType
        |> DataModel.CompetitorType.toString
        |> ste
        |> E.React.inH3
      }
    </>
  | {user: Some(r)} => r.name |> ste |> E.React.inH1
  | _ => notFound
  };

let component = ReasonReact.statelessComponent("AgentShow");

let make = (~id: string, _children) => {
  ...component,
  render: _ =>
    Queries.Agent.component(
      ~id,
      agent => {
        let mm =
          Queries.Agent.toMeasurables(
            agent.measurements |> E.A.O.concatSomes,
          );
        <>
          <SLayout.Header> {agentSection(agent)} </SLayout.Header>
          <SLayout.MainSection>
            {
              mm
              |> E.L.fmap((m: DataModel.Measurable.t) => {
                   let measurements = m.measurements |> E.O.default([]);
                   <>
                     <div className=block>
                       {C.Measurable.Items.link(~m)}
                       <C.Measurable.StatusDisplay
                         measurable=m
                         dateDisplay=WHOLE
                       />
                     </div>
                     {measurements |> C.Measurements.Table.make}
                   </>;
                 })
              |> E.A.of_list
              |> ReasonReact.array
            }
          </SLayout.MainSection>
        </>;
      },
    ),
};