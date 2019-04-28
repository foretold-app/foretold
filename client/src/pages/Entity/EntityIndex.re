open Utils;
open Rationale.Function.Infix;

let component = ReasonReact.statelessComponent("EntityShow");

module Columns = {
  type record = EKen.Thing.t;
  type column = Table.column(EKen.Thing.t);

  let nameColumn: column = {
    name: "Name" |> ste,
    render: (r: record) =>
      <Foretold__Components__Link
        linkType={Internal(EntityShow(r |> Graph_T.Thing.id))}>
        {r |> EKen.Thing.getName |> ste}
      </Foretold__Components__Link>,
  };

  let instanceOf: column = {
    name: "Instance Of" |> ste,
    render: (r: record) => r |> EKen.Thing.getInstanceOfName |> ste,
  };

  let idColumn: column = {
    name: "Name" |> ste,
    render: (r: record) => r |> Graph_T.Thing.id |> ste,
  };

  let all = [|nameColumn, instanceOf, idColumn|];
};

let dataSource = EKen.Things.getAll |> EKen.Things.withNames;

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("All Entities"),
      ~body=Table.fromColumns(Columns.all, dataSource),
    )
    |> layout,
};