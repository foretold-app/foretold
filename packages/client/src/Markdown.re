[@bs.config {jsx: 3}];

let component = ReasonReact.statelessComponent("Text");
let make = (~source, _children) => {
  ...component,
  render: _self => <ReactMarkdown.Markdown source />,
};

module Jsx3 = {
  [@bs.obj] external makeProps: (~source: string, unit) => _ = "";

  let make =
    ReasonReactCompat.wrapReasonReactForReact(
      ~component=ReasonReact.statelessComponent("Markdown"),
      (reactProps: {. "source": string}) =>
      make(~source=reactProps##source, [||])
    );
};
