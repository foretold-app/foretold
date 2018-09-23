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

let agentSection = (e: option(AgentTypes.agent)) =>
  switch (e) {
  | Some({bot: Some(r)}) =>
    <div>
      <h2> (r.name |> Option.default("") |> ste) </h2>
      <h3> (r.description |> Option.default("") |> ste) </h3>
      <h3> (r.competitorType |> botCompetitor |> ste) </h3>
    </div>
  | Some({user: Some(r)}) => <div> <h2> (r.name |> ste) </h2> </div>
  | _ => <div />
  };

let component = ReasonReact.statelessComponent("User");

let make = (~id: string, _children) => {
  ...component,
  render: _ => {
    let query = AgentTypes.GetAgent.make(~id, ());
    <div>
      <h2> (ReasonReact.string("Agent Page")) </h2>
      (
        AgentTypes.GetAgentQuery.make(~variables=query##variables, ({result}) =>
          result
          |> apolloResponseToResult
          <$> (
            e =>
              <div>
                (agentSection(e##agent))
                (
                  switch (e##agent) {
                  | Some(agent) =>
                    <AgentTable
                      measurements=(agent.measurements |> catOptionals)
                    />
                  | None => <div />
                  }
                )
              </div>
          )
          |> Result.result(idd, idd)
        )
        |> ReasonReact.element
      )
    </div>;
  },
};