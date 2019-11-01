[@bs.config {jsx: 3}];

module Styles = {
  open Css;
  let centerBlock = style([textAlign(`center), padding(`em(2.))]);
  let clear = style([clear(`both)]);
};

[@react.component]
let make = (~notebook: Types.notebook) => {
  <SLayout isFluid=true>
    <Padding>
      <FC__Div flexDirection=`row>
        <FC__Div flex={`num(5.)}>
          <FC__PageCard.H1> {notebook.name |> Utils.ste} </FC__PageCard.H1>
        </FC__Div>
        <FC__Div flex={`num(2.)}> <Null /> </FC__Div>
        <FC__Div flex={`num(1.)}>
          <AgentLink agent={notebook.owner} />
        </FC__Div>
        <FC__Div flex={`num(2.)}> <Null /> </FC__Div>
      </FC__Div>
    </Padding>
  </SLayout>;
};