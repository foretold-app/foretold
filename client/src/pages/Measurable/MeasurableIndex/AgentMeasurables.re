open Utils;
open Foretold__GraphQL;
let component = ReasonReact.statelessComponent("Measurables");
let make = (~id: string, ~loggedInUser: Queries.User.t, _children) => {
  ...component,
  render: _self =>
    Queries.Measurables.componentWithCreator(id)
    |> E.F.apply(measurables =>
         <MeasurableIndex__Table measurables loggedInUser showExtraData=true />
       ),
};