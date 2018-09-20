open Utils;
open Rationale;
open Rationale.Option;
open Result.Infix;
open Rationale.Function.Infix;
open Queries;
open HandsOnTable;

let toMeasurableLink = m => {
  let id = m.id;
  let name = m.name;
  {j|<a href="/measurables/$id">$name</a>|j};
};

/* e##createdAt |> Js.Json.decodeString |> Option.default(""), */
let showQueryList = (~data: Queries.measurables, ~urlFn, ~render) => {
  let ddata =
    Array.map(
      e =>
        Js.Dict.fromList([
          ("name", toMeasurableLink(e)),
          ("id", e.id),
          ("createdAt", e.createdAt),
        ]),
      data,
    );

  let columns = [|
    makeColumn(~data="name", ~renderer="html", ()),
    makeColumn(~data="createdAt", ()),
  |];

  <UseRouterForLinks>
    <HandsOnTable data=ddata columns colHeaders=[|"", "", ""|] />
  </UseRouterForLinks>;
};

let itemList =
  showQueryList(
    ~urlFn=e => "/measurables/" ++ e##id,
    ~render=e => e##name |> ste,
  );
/* Not sure why this needs optional */

let component = ReasonReact.statelessComponent("Measurables");
/* <div onClick={self.handleClick}> */
let make = _children => {
  ...component,
  render: self =>
    Queries.GetMeasurablesQuery.make(o =>
      o.result
      |> apolloResponseToResult
      <$> (d => d##measurables)
      <$> catOptionals
      <$> (e => <div> (itemList(~data=e)) </div>)
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element
    |> NormalLayout.make(~name="Measurables")
    |> ReasonReact.element,
};