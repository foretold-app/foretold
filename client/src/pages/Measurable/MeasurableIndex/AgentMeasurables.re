open Utils;
let component = ReasonReact.statelessComponent("Measurables");
let make = (~id: string, ~loggedInUser: GetUser.t, _children) => {
  ...component,
  render: _self =>
    GetMeasurables.componentWithCreator(id)
    |> E.F.apply(measurables =>
         <MeasurableIndex__Table measurables loggedInUser showExtraData=true />
       ),
};