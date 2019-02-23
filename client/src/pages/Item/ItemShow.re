open Utils;
open Rationale.Result.Infix;
open Rationale;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("ItemShow");

Data.run();
let make = (~id: string, _children) => {
  ...component,
  render: _ => {
    Js.log(
      Data.make |> Interface.Graph.findThing("@base/data-types/d-thing"),
    );
    <div> {id |> ste} </div>;
  },
};