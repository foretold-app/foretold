open Utils;
open Foretold__GraphQL;
open Css;

let block = style([marginBottom(`em(1.0))]);
let notFound = <h3> {"Agent not found" |> ste} </h3>;

let agentSection = (e: Queries.Agent.agent) =>
  switch (e) {
  | {bot: Some(r)} =>
    <>
      <h2> {r.name |> E.O.default("") |> ste} </h2>
      <h3> {r.description |> E.O.default("") |> ste} </h3>
      <h3> {r.competitorType |> DataModel.CompetitorType.toString |> ste} </h3>
    </>
  | {user: Some(r)} => <h1> {r.name |> ste} </h1>
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