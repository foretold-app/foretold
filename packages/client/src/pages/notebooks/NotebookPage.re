[@bs.config {jsx: 3}];

open Style.Grid;

type tab =
  | Dashboard
  | Details;

module Tabs = {
  open FC;

  [@react.component]
  let make = (~switchTab, ~tab) => {
    <Div>
      <Div
        styles=[
          Css.style([
            FC.Base.BaseStyles.floatLeft,
            Css.paddingTop(`em(0.2)),
          ]),
        ]>
        <TabButton
          isActive={tab == Dashboard} onClick={_ => switchTab(Dashboard)}>
          {"Dashboard" |> ReasonReact.string}
        </TabButton>
        <TabButton
          isActive={tab == Details} onClick={_ => switchTab(Details)}>
          {"Details" |> ReasonReact.string}
        </TabButton>
      </Div>
    </Div>;
  };
};

[@react.component]
let make = (~notebookPage: Routing.NotebookPage.t) => {
  let (tab, setTab) = React.useState(() => Dashboard);

  let switchTab = tabToSwitch => setTab(_ => tabToSwitch);
  let head = <Tabs switchTab tab />;

  <SLayout head isFluid=true>
    {switch (tab) {
     | Dashboard =>
       <Center> {"This is the notebook view" |> Utils.ste} </Center>
     | Details => <Center> {"This is the details view" |> Utils.ste} </Center>
     }}
  </SLayout>;
};