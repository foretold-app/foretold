open Table;
open Queries;
open Rationale;
open MomentRe;
open MeasurableTableBase;

let toCreatorLink = (c: Queries.creator) => {
  let id = c.id;
  let name = c.name;
  {j|<a href="/agents/$id">$name</a>|j};
};

let toMeasurableLink = m => {
  let id = m.id;
  let name = m.name;
  {j|<a href="/measurables/$id">$name</a>|j};
};

let toTableLink = (m: Queries.measurable) => {
  let item =
    MeasurableTables.Queries.find(m.measurableTableId |> Option.default(""));
  switch (item) {
  | Some(r) =>
    let id = r.id;
    let name = r.name;
    {j|<a href="/measurable-tables/$id">$name</a>|j};
  | None => ""
  };
};

let name =
  ColumnBundle.make(
    ~headerName="Name",
    ~get=toMeasurableLink,
    ~column=Columns.html,
    (),
  );

let nameColumn = {
  let toMeasurableLink = (id, name) => {j|<a href="/measurables/$id">$name</a>|j};
  ColumnBundle.make(
    ~headerName="Name",
    ~get=
      (e: Queries.measurable) =>
        toMeasurableLink(e.id, Queries.getFn(e, x => x.nameFn)),
    ~column=Columns.html,
    (),
  );
};

let descriptionColumn =
  ColumnBundle.make(
    ~headerName="Description",
    ~get=(e: Queries.measurable) => Queries.getFn(e, x => x.descriptionFn),
    (),
  );

let tableNameColumn =
  ColumnBundle.make(
    ~headerName="Table",
    ~get=(e: Queries.measurable) => toTableLink(e),
    ~column=Columns.html,
    (),
  );

let valueType =
  ColumnBundle.make(
    ~headerName="ValueType",
    ~get=
      e =>
        switch (e.valueType) {
        | `DATE => "Date"
        | `FLOAT => "Float"
        | `PERCENTAGE => "Percentage"
        },
    ~column=Columns.html,
    (),
  );

let measurementCount =
  ColumnBundle.make(
    ~headerName="Measurements",
    ~get=e => e.measurementCount |> Option.default(0) |> string_of_int,
    (),
  );

let measurerCount =
  ColumnBundle.make(
    ~headerName="Measurers",
    ~get=e => e.measurerCount |> Option.default(0) |> string_of_int,
    (),
  );

let isLocked =
  ColumnBundle.make(
    ~headerName="Is Locked",
    ~get=e => e.isLocked |> (r => r ? "True" : "False"),
    ~column=Columns.checkbox,
    (),
  );

let expectedResolutionDate =
  ColumnBundle.make(
    ~headerName="Expected Resolution Date",
    ~get=
      Option.Infix.(
        e =>
          e.expectedResolutionDate
          <$> Moment.format("L")
          |> Option.default("")
      ),
    (),
  );

let creator =
  ColumnBundle.make(
    ~headerName="Creator",
    ~get=
      Option.Infix.(
        e => {
          let creator = e.creator;
          creator <$> toCreatorLink |> Option.default("");
        }
      ),
    ~column=Columns.html,
    (),
  );