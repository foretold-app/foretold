let component = ReasonReact.statelessComponent("Text");

type code = {
  .
  "language": Js.nullable(string),
  "value": Js.nullable(string),
};

module Styles = {
  open Css;
  let all =
    style([
      selector(
        "h1, h2, h3, h4, h5, p, hr, blockquote, code",
        [
          maxWidth(`px(1170)),
          marginLeft(`auto),
          marginRight(`auto),
          display(`block),
        ],
      ),
      selector(
        "hr",
        [
          borderTop(`px(0), `solid, `hex("fff")),
          borderBottom(`px(1), `solid, `hex("ccc")),
        ],
      ),
    ]);
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
    | (Some(language), Some(value)) =>
      <code className=language> {value |> Utils.ste} </code>
    | (None, Some(value)) => value |> Utils.ste
    | (_, None) => E.React.null
    };
  },
};

let make = (~source, ~supportForetoldJs=false, ~channelId=?, _children) => {
  ...component,
  render: _self =>
    switch (supportForetoldJs, channelId) {
    | (true, Some(channelId)) =>
      <div className=Styles.all>
        <ReactMarkdown.Markdown
          source
          renderers={foretoldJsRenderers(channelId)}
        />
      </div>
    | (true, None) => <ReactMarkdown.Markdown source />
    | (false, _) => <ReactMarkdown.Markdown source />
    },
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