open Utils;
open Rationale;
open Rationale.Function.Infix;
open Result.Infix;

let botCompetitor = e =>
  switch (e) {
  | `AGGREGATION => "Aggregation"
  | `COMPETITIVE => "Competitive"
  | `OBJECTIVE => "Objective"
  };

let notFound = <h3> ("User not found" |> ste) </h3>;

let agentSection = (e: AgentTypes.agent) =>
  switch (e) {
  | {bot: Some(r)} =>
    <div>
      <h2> (r.name |> Option.default("") |> ste) </h2>
      <h3> (r.description |> Option.default("") |> ste) </h3>
      <h3> (r.competitorType |> botCompetitor |> ste) </h3>
    </div>
  | {user: Some(r)} => <div> <h2> (r.name |> ste) </h2> </div>
  | _ => notFound
  };

let component = ReasonReact.statelessComponent("AgentShow");

let withAgentQuery = (~id, innerFn) => {
  let query = AgentTypes.GetAgent.make(~id, ());
  AgentTypes.GetAgentQuery.make(~variables=query##variables, ({result}) =>
    result
    |> apolloResponseToResult
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
      <h2> ("Agent Page" |> ste) </h2>
      (
        withAgentQuery(~id, agent =>
          <div>
            (agentSection(agent))
            <AgentTable
              measurements=(agent.measurements |> ArrayOptional.concatSomes)
            />
          </div>
        )
      )
    </div>,
};