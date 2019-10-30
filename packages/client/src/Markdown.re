let component = ReasonReact.statelessComponent("Text");

type code = {
  .
  "language": Js.nullable(string),
  "value": Js.nullable(string),
};

type renderers = {. "code": code => ReasonReact.reactElement};

// switch (code##language, , code##value) {
let foretoldJsRenderers = (channelId): renderers => {
  "code": (code: code) => {
    switch (
      Js.Nullable.toOption(code##language),
      Js.Nullable.toOption(code##value),
    ) {
    | (Some("foretoldJs"), Some(json)) =>
      switch (Json.parse(json)) {
      | Some(json) => <DashboardTableC channelId tableJson=json />
      | None => "Invalid Json. Check a formatting tool." |> Utils.ste
      }
    | (Some(_), Some(value)) => value |> Utils.ste
    | (None, Some(value)) => value |> Utils.ste
    | (_, None) => E.React.null
    };
  },
};

let make = (~source, ~renderers=?, _children) => {
  ...component,
  render: _self => <ReactMarkdown.Markdown source ?renderers />,
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