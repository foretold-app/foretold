open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Rationale.Function.Infix;
open Queries;
open HandsOnTable;
open MomentRe;

let toMeasurableLink = m => {
  let id = m.id;
  let name = m.name;
  {j|<a href="/measurables/$id">$name</a>|j};
};

let showQueryList = (~data, ~urlFn, ~render) => {
  let ddata =
    Array.map(
      e =>
        Js.Dict.fromList([
          ("name", toMeasurableLink(e)),
          ("id", e.id),
          (
            "type",
            switch (e.valueType) {
            | `DATE => "Date"
            | `FLOAT => "Float"
            | `PERCENTAGE => "Percentage"
            },
          ),
          (
            "measurementCount",
            e.measurementCount |> Option.default(0) |> string_of_int,
          ),
          ("createdAt", e.createdAt |> Moment.format("L, h:mm:ss a")),
        ]),
      data,
    );

  let columns = [|
    makeColumn(~data="name", ~renderer="html", ()),
    makeColumn(~data="type", ()),
    makeColumn(~data="measurementCount", ()),
    makeColumn(~data="createdAt", ()),
  |];

  <UseRouterForLinks>
    <HandsOnTable
      data=ddata
      columns
      colHeaders=[|"Name", "Type", "Measurement Count", "Created At"|]
    />
  </UseRouterForLinks>;
};

let itemList =
  showQueryList(
    ~urlFn=e => "/measurables/" ++ e##id,
    ~render=e => e##name |> ste,
  );

let component = ReasonReact.statelessComponent("Measurables");
/* <div onClick={self.handleClick}> */
let make = _children => {
  ...component,
  render: self =>
    Queries.GetMeasurablesQuery.make(o =>
      o.result
      |> apolloResponseToResult
      <$> (d => d##measurables)
      <$> Extensions.Array.concatSomes
      <$> (e => <div> (itemList(~data=e)) </div>)
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element
    |> NormalLayout.make(~name="Measurables")
    |> ReasonReact.element,
};