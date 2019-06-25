module Styles = {
  open Css;
  let table = [display(`flex), flexDirection(`column)] |> style;
  let row = [paddingTop(`em(0.5)), paddingBottom(`em(0.5))] |> style;
  let cell = [flex(1), padding2(~v=`em(0.6), ~h=`em(0.5))] |> style;
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
  mutable showRowsCount: int,
  show: 'a => bool,
};

let countRows = (columns: array(column('a)), rows: array('a)) => {
  rows
  |> Array.iter((row: 'a) =>
       columns
       |> Array.iter((column: column('a)) =>
            column.showRowsCount =
              column.show(row)
                ? column.showRowsCount + 1 : column.showRowsCount
          )
     );
};

module Column = {
  let make =
      (~name, ~render, ~flex=1, ~showRowsCount=0, ~show=_ => true, ())
      : column('b) => {
    name,
    render,
    flex,
    showRowsCount,
    show,
  };
};

let fromColumns = (columns: array(column('a)), rows: array('a)) => {
  countRows(columns, rows);

  let columns' =
    columns
    |> Js.Array.filter(column => column.showRowsCount === 0 ? false : true);

  <Table>
    <FC.Table.HeaderRow>
      {columns'
       |> Array.map((column: column('a)) => column.name)
       |> Array.mapi((columenIndex, name) =>
            <FC.Table.Cell flex=1 key={columenIndex |> string_of_int}>
              name
            </FC.Table.Cell>
          )
       |> ReasonReact.array}
    </FC.Table.HeaderRow>
    {rows
     |> Array.mapi((rowIndex, row: 'a) =>
          <FC.Table.Row className=Styles.row key={rowIndex |> string_of_int}>
            {columns'
             |> Array.map((column: column('a)) => column.render(row))
             |> Array.map(renderedRow =>
                  <FC.Table.Cell flex=1> renderedRow </FC.Table.Cell>
                )
             |> ReasonReact.array}
          </FC.Table.Row>
        )
     |> ReasonReact.array}
  </Table>;
};