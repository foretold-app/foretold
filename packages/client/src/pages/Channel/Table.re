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
};

let fromColumns = (columns: array(column('a)), rows: array('a)) =>
  <Table>
    <FC.Table.HeaderRow>
      {columns
       |> Array.map((c: column('a)) => c.name)
       |> Array.mapi((index, name) =>
            <FC.Table.Cell flex=1 key={index |> string_of_int}>
              name
            </FC.Table.Cell>
          )
       |> ReasonReact.array}
    </FC.Table.HeaderRow>
    {rows
     |> Array.mapi((index, r: 'a) =>
          <FC.Table.Row className=Styles.row key={index |> string_of_int}>
            {columns
             |> Array.map((c: column('a)) => c.render(r))
             |> Array.map(renderedRow =>
                  <FC.Table.Cell flex=1> renderedRow </FC.Table.Cell>
                )
             |> ReasonReact.array}
          </FC.Table.Row>
        )
     |> ReasonReact.array}
  </Table>;