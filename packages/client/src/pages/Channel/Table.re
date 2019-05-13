open Utils;

module Styles = {
  open Css;
  let table = [display(`flex), flexDirection(`column)] |> style;
  let row =
    [
      flex(1),
      display(`flex),
      borderBottom(`px(1), `solid, `hex("e7eaf0")),
      selector(
        ":last-child",
        [borderBottom(`px(0), `solid, hex("e7eaf0"))],
      ),
    ]
    |> style;
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
        {
          cells
          |> E.A.fmapi((i, r: ReasonReact.reactElement) =>
               <Cell key={i |> string_of_int}> r </Cell>
             )
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
      |> E.A.fmapi((i, r: 'a) =>
           <Row
             cells={columns |> E.A.fmap((c: column('a)) => c.render(r))}
             key={i |> string_of_int}
           />
         )
      |> ReasonReact.array
    }
  </Table>;