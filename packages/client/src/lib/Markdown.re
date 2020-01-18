type code = {
  .
  "language": Js.nullable(string),
  "value": Js.nullable(string),
};

type link = {
  .
  "href": Js.nullable(string),
  "children": Js.nullable(array(React.element)),
};

let foretoldJsRenderers = entityGraph => {
  module Config = {
    let entityGraph = entityGraph;
  };
  module Ken = KenTools.Functor(Config);
  {
    "link": (link: link) => {
      switch (
        Js.Nullable.toOption(link##href),
        Js.Nullable.toOption(link##children),
      ) {
      | (Some(a), Some(b)) when Js.String.startsWith("@", a) =>
        <AntdPopover
          content={Ken.subjectIdToDisplay(a)} trigger=`hover placement=`top>
          <a href={Ken.itemUrl(a)}>
            {Ken.findName(a)
             |> E.O.fmap(r => r |> Utils.ste)
             |> E.O.default(b |> React.array)}
          </a>
        </AntdPopover>
      | (Some(a), Some(b)) => <a href=a> {b |> React.array} </a>
      | _ => React.null
      };
    },
  };
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
  <Providers.AppContext.Consumer>
    ...{context => {
      let entityGraph =
        context.globalSetting
        |> E.O.fmap((e: Types.globalSetting) => e.entityGraph)
        |> E.O.default(None);
      <ReactMarkdown
        source={Js.String.trim(source)}
        renderers={foretoldJsRenderers(entityGraph)}
      />;
    }}
  </Providers.AppContext.Consumer>;
};