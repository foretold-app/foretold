[@bs.module "@handsontable/react"]
external handsOnTable : ReasonReact.reactClass = "HotTable";

[@bs.deriving abstract]
type column = {
  data: string,
  renderer: string,
  readOnly: bool,
};

let makeColumn =
    (~name: string, ~renderer: string="text", ~readOnly: bool=true, ())
    : column =>
  column(~data=name, ~renderer, ~readOnly);

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
      "stretchH": "all",
    },
    children,
  );