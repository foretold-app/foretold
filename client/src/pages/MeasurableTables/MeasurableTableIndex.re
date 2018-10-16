open MeasurableTables;

let component = ReasonReact.statelessComponent("MeasurableTableIndex");

let transformations = [
  Columns.name,
  Columns.description,
  Columns.schemaNameN(0),
  Columns.schemaTypeN(0),
  Columns.schemaNameN(1),
  Columns.schemaTypeN(1),
  Columns.schemaNameN(2),
  Columns.schemaTypeN(2),
];

let make = _children => {
  ...component,
  render: _ =>
    MeasurableTables.all
    |> Table.ColumnBundle.toHOT(~data=_, ~transformations, ()),
};