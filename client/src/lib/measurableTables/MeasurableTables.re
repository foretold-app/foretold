open Table;

module Base = {
  include MeasurableTableBase;
};

let all = MeasurableTableItems.all;

module Queries = {
  let find = (id: string) =>
    MeasurableTableItems.all
    |> Array.to_list
    |> Rationale.RList.find((r: MeasurableTableBase.measurableType) =>
         r.id == id
       );
};

module Columns = {
  open MeasurableTableBase;

  let toNameLink = (m: measurableType) => {
    let id = m.id;
    let name = m.name;
    {j|<a href="/measurable-tables/$id">$name</a>|j};
  };

  let toSchemaName = (i: int, m: measurableType) => {
    let elem = Rationale.RList.nth(i, m.schema);
    switch (elem) {
    | None => ""
    | Some(e) => e.name
    };
  };

  let toSchemaType = (i: int, m: measurableType) => {
    let elem = Rationale.RList.nth(i, m.schema);
    switch (elem) {
    | None => ""
    | Some(e) =>
      switch (e.t) {
      | `Date => "Date"
      | `Entity => "Entity"
      | `String => "String"
      }
    };
  };

  let name =
    ColumnBundle.make(
      ~headerName="Name",
      ~get=toNameLink,
      ~column=Columns.html,
      (),
    );

  let id = ColumnBundle.make(~headerName="Id", ~get=e => e.name, ());

  let description =
    ColumnBundle.make(~headerName="Description", ~get=e => e.description, ());

  let schemaNameN = i =>
    ColumnBundle.make(
      ~headerName="Col-" ++ string_of_int(i) ++ " Name",
      ~get=toSchemaName(i),
      (),
    );

  let schemaTypeN = i =>
    ColumnBundle.make(
      ~headerName="Col-" ++ string_of_int(i) ++ " Type",
      ~get=toSchemaType(i),
      (),
    );
};