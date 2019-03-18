open Utils;
let component = ReasonReact.statelessComponent("Measurables");
let make = (~id: string, ~loggedInUser: GetUser.t, _children) => {
  ...component,
  render: _self =>
    (
      measurables =>
        <MeasurableIndex__Table measurables loggedInUser showExtraData=true />
    )
    |> GetMeasurables.componentWithCreator(id),
};