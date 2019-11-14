[@bs.config {jsx: 3}];

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
        "h1, h2, h3, h4, h5, p, blockquote, code",
        [
          maxWidth(`px(682)),
          marginLeft(`auto),
          marginRight(`auto),
          display(`block),
          paddingLeft(`rem(1.)),
          paddingRight(`rem(1.)),
        ],
      ),
      selector(
        "ul, ol",
        [
          maxWidth(`px(682)),
          marginLeft(`auto),
          marginRight(`auto),
          paddingLeft(`rem(2.)),
          paddingRight(`rem(1.)),
        ],
      ),
      selector(
        "blockquote",
        [
          borderLeft(`px(4), `solid, `hex("9399a3")),
          marginTop(`rem(1.5)),
          marginBottom(`rem(1.5)),
        ],
      ),
      selector(
        "hr",
        [
          maxWidth(`px(682)),
          marginLeft(`auto),
          marginRight(`auto),
          display(`block),
          borderTop(`px(0), `solid, `hex("fff")),
          borderBottom(`px(1), `solid, `hex("ccc")),
        ],
      ),
      selector("img", [maxHeight(`px(900)), maxWidth(`percent(100.))]),
    ]);
};

type renderers = {. "code": code => ReasonReact.reactElement};

// switch (code##language, , code##value) {
let foretoldJsRenderers = {
  "code": (code: code) => {
    switch (
      Js.Nullable.toOption(code##language),
      Js.Nullable.toOption(code##value),
    ) {
    | (Some("foretoldJs"), Some(json)) =>
      switch (Json.parse(json)) {
      | Some(json) =>
        <div
          className=Css.(
            style([
              marginTop(`em(1.0)),
              marginBottom(`em(1.5)),
              Css.float(`left),
              width(`percent(100.)),
            ])
          )>
          <DashboardTableC tableJson=json />
        </div>
      | None => "Invalid Json. Check a formatting tool." |> Utils.ste
      }
    | (Some(language), Some(value)) =>
      <code className=language> {value |> Utils.ste} </code>
    | (None, Some(value)) => value |> Utils.ste
    | (_, None) => E.React2.null
    };
  },
};

[@react.component]
let make = (~source, ~supportForetoldJs=false) => {
  supportForetoldJs
    ? <div className=Styles.all>
        <ReactMarkdown source renderers=foretoldJsRenderers />
      </div>
    : <ReactMarkdown source />;
};