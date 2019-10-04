[@bs.config {jsx: 3}];

[@react.component]
let make = (~text, ~children) =>
  <div> {text |> Utils.ste} {children |> ReasonReact.array} </div>;

// Read docs how to Step by step gone into PPX3
// Docs: https://reasonml.github.io/reason-react/docs/en/jsx#upgrade-script
// Usages: <TestJsx3.Jsx2 text="Text from props"> {"Children Text" |> Utils.ste} </TestJsx3.Jsx2>

module Jsx2 = {
  let component = ReasonReact.statelessComponent("TestJsx2");
  /* `children` is not labelled, as it is a regular parameter in version 2 of JSX */
  let make = (~text, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~text, ~children, ()),
      children,
    );
};
