open Utils;
open HandsOnTable;
let component = ReasonReact.statelessComponent("NormalLayout");

let make = (~name="Add a name!", children) => {
  ...component,
  render: _ =>
    <div> <Header /> <h2> (ReasonReact.string(name)) </h2> children </div>,
};