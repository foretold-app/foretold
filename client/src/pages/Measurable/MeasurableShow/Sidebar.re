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
  <a href=("/measurables/" ++ id)> (name |> ste) </a>;
};

module Styles = {
  open Css;
  let borderColor = hex("aaaaaa");
  let item = isSelected =>
    style([
      color(black),
      cursor(`pointer),
      backgroundColor(isSelected ? hex("a2cdfb") : white),
      width(`percent(100.0)),
      padding2(~v=px(10), ~h=px(10)),
      borderBottom(px(1), solid, borderColor),
      minHeight(px(50)),
      firstChild([borderTop(px(1), solid, borderColor)]),
      hover([backgroundColor(hex("dcedff"))]),
    ]);

  let body = style([marginLeft(px(200))]);
};
/* className=(Styles.item(~isSelected=selectedId == e.id)) */

let urlFn = e => "/measurables/" ++ e.id;
let showQueryList = (~data: Queries.measurables, ~selectedId: string) =>
  Array.mapi(
    (index, e) =>
      <div
        className=(Styles.item(selectedId == e.id))
        key=(string_of_int(index))
        onClick=(_event => e |> urlFn |> ReasonReact.Router.push)>
        (e.name |> ste)
      </div>,
    data,
  )
  |> ReasonReact.array;

/* Not sure why this needs optional */

let component = ReasonReact.statelessComponent("Measurables");
/* <div onClick={self.handleClick}> */
let make = (~selectedId: string, _children) => {
  ...component,
  render: self =>
    Queries.GetMeasurablesQuery.make(o =>
      o.result
      |> apolloResponseToResult
      <$> (d => d##measurables)
      <$> catOptionals
      <$> (e => <div> (showQueryList(~data=e, ~selectedId)) </div>)
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element,
};