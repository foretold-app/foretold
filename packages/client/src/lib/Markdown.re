type code = {
  .
  "language": Js.nullable(string),
  "value": Js.nullable(string),
};

module Styles = {
  open Css;

  let all =
    style([
      selector("h1, h2, h3, h4, h5", [lineHeight(`em(1.4))]),
      selector(
        "pre",
        [
          background(`hex("f4f6f9")),
          borderRadius(`px(3)),
          paddingTop(`em(0.6)),
          paddingBottom(`em(0.6)),
          marginTop(`em(1.)),
          marginBottom(`em(1.)),
        ],
      ),
      selector(
        "h1, h2, h3, h4, h5, p, blockquote, pre",
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

[@react.component]
let make = (~source, ~supportForetoldJs=false) => {
  <ReactMarkdown source={Js.String.trim(source)} />;
};