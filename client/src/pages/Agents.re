open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Queries;
open HandsOnTable;

let toAgentLink = (id, name) => {j|<a href="/agents/$id">$name</a>|j};

let component = ReasonReact.statelessComponent("Measurables");
let perEl = (e: Queries.agent) =>
  switch (e) {
  | {bot: Some(r)} =>
    Js.Dict.fromList([
      ("name", r.name |> Option.default("") |> toAgentLink(e.id)),
      ("type", "Bot"),
      ("description", r.description |> Option.default("")),
      ("competitorType", Queries.stringOfcompetitorType(r.competitorType)),
    ])
  | {user: Some(r)} =>
    Js.Dict.fromList([
      ("name", r.name |> toAgentLink(e.id)),
      ("type", "User"),
      ("competitorType", "All"),
    ])
  | _ => Js.Dict.fromList([])
  };

let make = _children => {
  ...component,
  render: _ =>
    <div>
      (
        Queries.GetAgentsQuery.make(({result}) =>
          result
          |> apolloResponseToResult
          <$> (d => d##agents)
          <$> (
            e => {
              let data = Array.map(perEl, e |> catOptionals);
              let columns = [|
                makeColumn(~data="name", ~renderer="html", ()),
                makeColumn(~data="type", ()),
                makeColumn(~data="competitorType", ()),
                makeColumn(~data="description", ()),
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