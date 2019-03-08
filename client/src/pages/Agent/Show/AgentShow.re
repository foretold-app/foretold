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
      <h2> {r.name |> Option.default("") |> ste} </h2>
      <h3> {r.description |> Option.default("") |> ste} </h3>
      <h3> {r.competitorType |> botCompetitor |> ste} </h3>
    </div>
  | {user: Some(r)} => <div> <h1> {r.name |> ste} </h1> </div>
  | _ => notFound
  };

let component = ReasonReact.statelessComponent("AgentShow");

let withAgentQuery = (~id, innerFn) => {
  let query = AgentTypes.GetAgent.make(~id, ());
  AgentTypes.GetAgentQuery.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    <$> (e => e##agent)
    >>= (
      e =>
        switch (e) {
        | Some(a) => Ok(a)
        | None => Error(notFound)
        }
    )
    <$> innerFn
    |> Result.result(idd, idd)
  )
  |> ReasonReact.element;
};
let make = (~id: string, _children) => {
  ...component,
  render: _ =>
    <div>
      {
        withAgentQuery(
          ~id,
          agent => {
            let mm =
              AgentTypes.toMeasurables(
                agent.measurements |> ArrayOptional.concatSomes,
              );
            <div>
              {agentSection(agent)}
              {
                mm
                |> List.map((m: DataModel.measurable) => {
                     let measurements =
                       m.measurements |> Rationale.Option.default([]);
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
                |> Array.of_list
                |> ReasonReact.array
              }
            </div>;
          },
        )
      }
    </div>,
};