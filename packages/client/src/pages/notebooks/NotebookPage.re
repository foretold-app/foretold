[@bs.config {jsx: 3}];

open Style.Grid;

type tab =
  | Show
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
        <TabButton isActive={tab == Show} onClick={_ => switchTab(Show)}>
          {"Notebook" |> Utils.ste}
        </TabButton>
        <TabButton
          isActive={tab == Details} onClick={_ => switchTab(Details)}>
          {"Details" |> Utils.ste}
        </TabButton>
      </Div>
    </Div>;
  };
};

[@react.component]
let make = (~channelId: string, ~notebookId: string) => {
  let (tab, setTab) = React.useState(() => Show);
  let switchTab = tabToSwitch => setTab(_ => tabToSwitch);

  let head = <Tabs switchTab tab />;
  NotebookGet.component(~id=notebookId, notebook =>
    switch (notebook) {
    | Some(notebook) =>
      <>
        <NotebookHeader notebook />
        <SLayout head isFluid=true>
          {switch (tab) {
           | Show =>
             <div
               className=Css.(
                 style([marginTop(`em(2.0)), marginBottom(`em(2.0))])
               )>
               <Markdown
                 source={notebook.body}
                 supportForetoldJs=true
                 channelId
               />
             </div>
           | Details =>
             <Center> {"This is the details view" |> Utils.ste} </Center>
           }}
        </SLayout>
      </>
    | _ => <NotFoundPage />
    }
  );
};