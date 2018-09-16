[@bs.module "@handsontable/react"]
external handsOnTable : ReasonReact.reactClass = "HotTable";

[@bs.deriving abstract]
type column = {
  data: string,
  renderer: string,
  readOnly: bool,
};

let makeColumn =
    (~data: string, ~renderer: string="text", ~readOnly: bool=true, ())
    : column =>
  column(~data, ~renderer, ~readOnly);

let foo = [|Js.Dict.fromList([("aa", "bar"), ("bb", "charr")])|];

let bar = [|
  column(~data="aa", ~readOnly=true),
  column(~data="bb", ~readOnly=true),
|];

let make =
    (
      ~data: array(Js.Dict.t(string)),
      ~columns: array(column),
      ~colHeaders: array(string),
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=handsOnTable,
    ~props={
      "data": data,
      "columns": columns,
      "colHeaders": colHeaders,
      "rowHeaders": true,
      "columnSorting": true,
      "sortIndicator": true,
      "width": "1000",
      "height": "800",
      "stretchH": "all",
    },
    children,
  );