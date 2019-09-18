open Style.Grid;

let component = ReasonReact.statelessComponent("MeasurementsXx");
let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    <> {"xx" |> Utils.ste} </>;
  },
};