type column('a) = {
  name: ReasonReact.reactElement,
  render: 'a => ReasonReact.reactElement,
  help: option(ForetoldComponents.HelpDropdown.content),
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
  let make = (~name, ~render, ~flex=1, ~show=_ => true, ~help=None, ()) => {
    name,
    help,
    render,
    flex,
    show,
  };
};

[@react.component]
let make = (~columns, ~rows, ~bottomSubRowFn=None, ~onRowClb=None) => {
  let columns' = filterColums(columns, rows);

  <ForetoldComponents.Table>
    <ForetoldComponents.Table.HeaderRow>
      {columns'
       |> E.A.fmapi((columnIndex, column: column('a)) =>
            <ForetoldComponents.Table.Cell
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
                     <ForetoldComponents.HelpDropdown content />
                   </span>
                 </span>
               }}
            </ForetoldComponents.Table.Cell>
          )
       |> ReasonReact.array}
    </ForetoldComponents.Table.HeaderRow>
    {rows
     |> E.A.fmapi((rowIndex, row: 'a) => {
          let columnsBody =
            columns'
            |> E.A.fmapi((columnIndex, column: column('a)) =>
                 <ForetoldComponents.Table.Cell
                   flex={`num(column.flex |> float_of_int)}
                   key={columnIndex |> string_of_int}>
                   {column.render(row)}
                 </ForetoldComponents.Table.Cell>
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

          <ForetoldComponents.Table.Row ?onClick ?bottomSubRow key>
            columnsBody
          </ForetoldComponents.Table.Row>;
        })
     |> ReasonReact.array}
  </ForetoldComponents.Table>;
};