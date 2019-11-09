[@bs.config {jsx: 3}];

module Styles = {
  open Css;
  let main =
    style([
      maxWidth(`px(1170)),
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
      color(`hex("192D44")),
      marginBottom(`em(0.3)),
      FC__Settings.FontWeights.heavy,
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
    <FC__Div flexDirection=`row>
      <FC__Div flex={`num(5.)}>
        <h1 className=Styles.h1> {notebook.name |> Utils.ste} </h1>
      </FC__Div>
      <FC__Div flex={`num(3.)} />
      <FC__Div flex={`num(1.)}>
        <AgentLink agent={notebook.owner} />
      </FC__Div>
    </FC__Div>
  </div>;
};