module Styles = {
  open Css;
  let main =
    style([
      maxWidth(`px(682)),
      marginLeft(`auto),
      marginRight(`auto),
      display(`block),
      paddingLeft(`rem(1.)),
      paddingRight(`rem(1.)),
      marginTop(`em(1.5)),
      marginBottom(`em(2.5)),
    ]);
  let h1 =
    style([
      fontSize(`em(1.9)),
      lineHeight(`em(1.3)),
      color(`hex("192D44")),
      marginBottom(`em(0.5)),
      ForetoldComponents.Settings.FontWeights.heavy,
    ]);
  let hr =
    style([
      borderTop(`px(0), `solid, `hex("fff")),
      borderBottom(`px(1), `solid, `hex("8C9EB540")),
    ]);
};

[@react.component]
let make = (~notebook: Types.notebook) => {
  <div className=Styles.main>
    <ForetoldComponents.Div flexDirection=`column>
      <ForetoldComponents.Div flex={`num(1.)}>
        <h1 className=Styles.h1> {notebook.name |> Utils.ste} </h1>
      </ForetoldComponents.Div>
      <ForetoldComponents.Div flex={`num(1.)}>
        <AgentLink agent={notebook.owner} />
      </ForetoldComponents.Div>
    </ForetoldComponents.Div>
  </div>;
};