open Utils;
open Foretold__GraphQL;

let notFound = <h3> {"Agent not found" |> ste} </h3>;

let agentSection = (e: AgentGet.agent) =>
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
    AgentGet.component(
      ~id,
      agent => {
        let mm =
          AgentGet.toMeasurables(agent.measurements |> E.A.O.concatSomes);
        <>
          <SLayout.Header> {agentSection(agent)} </SLayout.Header>
          <SLayout.MainSection>
            {
              mm
              |> E.L.fmap((m: DataModel.Measurable.t) => {
                   let measurements = m.measurements |> E.O.default([]);
                   <>
                     <div className=AgentShowStyles.block>
                       {MeasurableTableStyles.link(~m)}
                       {MeasurableTableStyles.dateStatus(~measurable=m)}
                     </div>
                     <div className=MeasurementTableStyles.group>
                       {measurements |> MeasurementsBlock.make}
                     </div>
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