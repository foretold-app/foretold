open Utils;
let component = ReasonReact.statelessComponent("Bar");
let make = children => {
  ...component,
  shouldUpdate: a => false,
  render: _ => <div> ...children </div>,
};