open Utils;

module Styles = {
  open Css;
  let table =
    [display(`flex), flexWrap(`wrap), flexDirection(`column)] |> style;
  let row = [flex(1), display(`flex), flexDirection(`row)] |> style;
  let cell = [flex(1)] |> style;
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
        {
          cells
          |> E.A.fmap((r: ReasonReact.reactElement) => <Cell> r </Cell>)
          |> ReasonReact.array
        }
      </div>,
  };
};

type column('a) = {
  name: ReasonReact.reactElement,
  render: 'a => ReasonReact.reactElement,
};

let fromColumns = (columns: array(column('a)), rows: array('a)) =>
  <Table>
    <Row cells={columns |> E.A.fmap((c: column('a)) => c.name)} />
    {
      rows
      |> E.A.fmap((r: 'a) =>
           <Row
             cells={columns |> E.A.fmap((c: column('a)) => c.render(r))}
           />
         )
      |> ReasonReact.array
    }
  </Table>;