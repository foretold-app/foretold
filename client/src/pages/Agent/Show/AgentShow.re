open Utils;
open Rationale;
open Result.Infix;

let botCompetitor = e =>
  switch (e) {
  | `AGGREGATION => "Aggregation"
  | `COMPETITIVE => "Competitive"
  | `OBJECTIVE => "Objective"
  };

let notFound = <h3> {"Agent not found" |> ste} </h3>;

let agentSection = (e: AgentTypes.agent) =>
  switch (e) {
  | {bot: Some(r)} =>
    <div>
      <h2> {r.name |> E.O.default("") |> ste} </h2>
      <h3> {r.description |> E.O.default("") |> ste} </h3>
      <h3> {r.competitorType |> botCompetitor |> ste} </h3>
    </div>
  | {user: Some(r)} => <h1> {r.name |> ste} </h1>
  | _ => notFound
  };

let component = ReasonReact.statelessComponent("AgentShow");

let make = (~id: string, _children) => {
  ...component,
  render: _ =>
    <div>
      {
        AgentTypes.GetAgent.component(
          ~id,
          agent => {
            let mm =
              AgentTypes.toMeasurables(
                agent.measurements |> E.A.Optional.concatSomes,
              );
            <div>
              <SLayout.Header> {agentSection(agent)} </SLayout.Header>
              <SLayout.MainSection>
                {
                  mm
                  |> E.L.fmap((m: DataModel.measurable) => {
                       let measurements = m.measurements |> E.O.default([]);
                       <div>
                         <div className=AgentShowStyles.block>
                           {MeasurableTableStyles.link(~m)}
                           {MeasurableTableStyles.dateStatus(~measurable=m)}
                         </div>
                         <div className=MeasurementTableStyles.group>
                           {measurements |> MeasurementsBlock.make}
                         </div>
                       </div>;
                     })
                  |> E.A.of_list
                  |> ReasonReact.array
                }
              </SLayout.MainSection>
            </div>;
          },
        )
      }
    </div>,
};