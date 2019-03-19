open Utils;
open Foretold__GraphQL;
let component = ReasonReact.statelessComponent("Measurables");
let make = (~id: string, ~loggedInUser: UserGet.t, _children) => {
  ...component,
  render: _self =>
    MeasurablesGet.componentWithCreator(id)
    |> E.F.apply(measurables =>
         <MeasurableIndex__Table measurables loggedInUser showExtraData=true />
       ),
};