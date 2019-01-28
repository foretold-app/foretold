open Table;
open DataModel;
open Rationale;
open MomentRe;

let toCreatorLink = (c: DataModel.creator) => {
  let id = c.id;
  let name = c.name;
  {j|<a href="/agents/$id">$name</a>|j};
};

let toMeasurableLink = (m: DataModel.measurable) => {
  let id = m.id;
  let name = m.name;
  {j|<a href="/measurables/$id">$name</a>|j};
};

let toSimpleLink = (m: DataModel.measurable) => {
  let id = m.id;
  {j|<a href="/measurables/$id">Link</a>|j};
};

let link =
  ColumnBundle.make(
    ~headerName="Link",
    ~get=toSimpleLink,
    ~column=Columns.html,
    (),
  );

let nameAsText = ColumnBundle.make(~headerName="Name", ~get=e => e.name, ());

let name =
  ColumnBundle.make(
    ~headerName="Name",
    ~get=toMeasurableLink,
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