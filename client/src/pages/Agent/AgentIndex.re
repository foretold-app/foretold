open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Queries;
open HandsOnTable;

let toAgentLink = (id, name) => {j|<a href="/agents/$id">$name</a>|j};

let component = ReasonReact.statelessComponent("Measurables");
let perEl = (e: Queries.agent) => {
  let count =
    switch (e) {
    | {measurementCount: Some(m)} => string_of_int(m)
    | _ => ""
    };
  switch (e) {
  | {bot: Some(r)} =>
    Js.Dict.fromList([
      ("name", r.name |> Option.default("") |> toAgentLink(e.id)),
      ("type", "Bot"),
      ("description", r.description |> Option.default("")),
      ("competitorType", Queries.stringOfcompetitorType(r.competitorType)),
      ("measurementCount", count),
    ])
  | {user: Some(r)} =>
    Js.Dict.fromList([
      ("name", r.name |> toAgentLink(e.id)),
      ("type", "User"),
      ("competitorType", "All"),
      ("measurementCount", count),
    ])
  | _ => Js.Dict.fromList([])
  };
};

let make = _children => {
  ...component,
  render: _ =>
    <div>
      <h2> ("Agents" |> ste) </h2>
      (
        Queries.GetAgentsQuery.make(({result}) =>
          result
          |> apolloResponseToResult
          <$> (d => d##agents)
          <$> (
            e => {
              let data = Array.map(perEl, e |> ArrayOptional.concatSomes);
              let columns = [|
                makeColumn(~name="name", ~renderer="html", ()),
                makeColumn(~name="type", ()),
                makeColumn(~name="competitorType", ()),
                makeColumn(~name="description", ()),
                makeColumn(~name="measurementCount", ()),
              |];
              <div>
                <UseRouterForLinks>
                  <HandsOnTable
                    data
                    columns
                    colHeaders=[|
                      "Name",
                      "Type",
                      "CompetitorType",
                      "Description",
                      "Measurements",
                    |]
                  />
                </UseRouterForLinks>
              </div>;
            }
          )
          |> Result.result(idd, idd)
        )
        |> ReasonReact.element
      )
    </div>,
};