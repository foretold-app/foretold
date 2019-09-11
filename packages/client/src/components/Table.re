module Styles = {
  open Css;
  let table = [display(`flex), flexDirection(`column)] |> style;
  let row = [paddingTop(`em(0.5)), paddingBottom(`em(0.5))] |> style;
  let cell =
    [flex(`num(1.)), padding2(~v=`em(0.6), ~h=`em(0.5))] |> style;
};

module Table = {
  let component = ReasonReact.statelessComponent("Table");
  let make = children => {
    ...component,
    render: _self => <div className=Styles.table> ...children </div>,
  };
};

module Cell = {
  let component = ReasonReact.statelessComponent("Cell");
  let make = children => {
    ...component,
    render: _self => <div className=Styles.cell> ...children </div>,
  };
};

module Row = {
  let component = ReasonReact.statelessComponent("Row");
  let make = (~cells: array(ReasonReact.reactElement), _) => {
    ...component,
    render: _self =>
      <div className=Styles.row>
        {cells
         |> Array.mapi((index, cellBody: ReasonReact.reactElement) =>
              <Cell key={index |> string_of_int}> cellBody </Cell>
            )
         |> ReasonReact.array}
      </div>,
  };
};

type column('a) = {
  name: ReasonReact.reactElement,
  render: 'a => ReasonReact.reactElement,
  flex: int,
  show: 'a => bool,
};

let filterColums = (columns, rows) => {
  columns
  |> Js.Array.filter(column =>
       rows |> Js.Array.find(column.show) |> E.O.toBool
     );
};

module Column = {
  let make = (~name, ~render, ~flex=1, ~show=_ => true, ()): column('b) => {
    name,
    render,
    flex,
    show,
  };
};

let fromColumns =
    (
      columns: array(column('a)),
      rows: array('a),
      ~bottomSubRowFn: option('a => option(array(ReasonReact.reactElement)))=None,
      ~onRowClb: 'a => unit=_ => (),
      (),
    ) => {
  let columns' = filterColums(columns, rows);

  <Table>
    <Fc.FC__Table.HeaderRow>
      {columns'
       |> Array.mapi((columnIndex, column: column('a)) =>
            <Fc.FC.Table.Cell
              flex={`num(column.flex |> float_of_int)}
              key={columnIndex |> string_of_int}>
              {column.name}
            </Fc.FC.Table.Cell>
          )
       |> ReasonReact.array}
    </Fc.FC__Table.HeaderRow>
    {rows
     |> Array.mapi((rowIndex, row: 'a) => {
          let columnsBody =
            columns'
            |> Array.mapi((columnIndex, column: column('a)) =>
                 <Fc.FC.Table.Cell
                   flex={`num(column.flex |> float_of_int)}
                   key={columnIndex |> string_of_int}>
                   {column.render(row)}
                 </Fc.FC.Table.Cell>
               )
            |> ReasonReact.array;

          let key = rowIndex |> string_of_int;
          let bottomSubRow = bottomSubRowFn |> E.O.bind(_, r => r(row));

          <Fc.FC.Table.Row
            onClick={_ => {
              onRowClb(row);
              ();
            }}
            className=Styles.row
            ?bottomSubRow
            key>
            columnsBody
          </Fc.FC.Table.Row>;
        })
     |> ReasonReact.array}
  </Table>;
};