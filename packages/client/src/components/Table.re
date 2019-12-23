type column('a) = {
  name: ReasonReact.reactElement,
  render: 'a => ReasonReact.reactElement,
  help: option(FC.HelpDropdown.content),
  flex: int,
  show: 'a => bool,
};

let headerCellStyles =
  Css.[
    paddingTop(`em(0.2)),
    paddingBottom(`em(0.2)),
    fontSize(`em(0.9)),
  ];

let headerCellPadding =
  Css.(
    style([
      paddingTop(`em(0.3)),
      paddingBottom(`em(0.4)),
      background(`hex("000000")),
    ])
  );

let filterColums = (columns, rows) => {
  columns
  |> Js.Array.filter(column =>
       rows |> Js.Array.find(column.show) |> E.O.toBool
     );
};

module Column = {
  let make =
      (~name, ~render, ~flex=1, ~show=_ => true, ~help=None, ()): column('b) => {
    name,
    help,
    render,
    flex,
    show,
  };
};

[@react.component]
let make =
    (
      ~columns: array(column('a)),
      ~rows: array('a),
      ~bottomSubRowFn=None,
      ~onRowClb=None,
    ) => {
  let columns' = filterColums(columns, rows);

  <FC__Table>
    <FC__Table.HeaderRow>
      {columns'
       |> Array.mapi((columnIndex, column: column('a)) =>
            <FC.Table.Cell
              flex={`num(column.flex |> float_of_int)}
              properties=headerCellStyles
              key={columnIndex |> string_of_int}>
              {switch (column.help) {
               | None => column.name
               | Some(content) =>
                 <span>
                   {column.name}
                   <span
                     className=Css.(
                       style([fontSize(`em(1.2)), marginLeft(`em(0.4))])
                     )>
                     <FC.HelpDropdown content />
                   </span>
                 </span>
               }}
            </FC.Table.Cell>
          )
       |> ReasonReact.array}
    </FC__Table.HeaderRow>
    {rows
     |> Array.mapi((rowIndex, row: 'a) => {
          let columnsBody =
            columns'
            |> Array.mapi((columnIndex, column: column('a)) =>
                 <FC.Table.Cell
                   flex={`num(column.flex |> float_of_int)}
                   key={columnIndex |> string_of_int}>
                   {column.render(row)}
                 </FC.Table.Cell>
               )
            |> ReasonReact.array;

          let key = rowIndex |> string_of_int;
          let bottomSubRow =
            bottomSubRowFn
            |> E.O.bind(_, r => r(row))
            |> E.O.fmap(ReasonReact.array);
          let onClick =
            onRowClb
            |> E.O.fmap((r, _) => {
                 r(row);
                 ();
               });

          <FC.Table.Row ?onClick ?bottomSubRow key>
            columnsBody
          </FC.Table.Row>;
        })
     |> ReasonReact.array}
  </FC__Table>;
};