open Style.Grid;

module Styles = {
  open Css;
  let full = style([float(`left), width(`percent(100.))]);
  let sidebarOutside =
    style([background(`hex("f0f2f5")), paddingLeft(`em(1.0))]);
  let sidebar = style([position(`sticky), top(`em(2.0)), float(`left)]);
};

type tab =
  | Show
  | Edit
  | Details;

// @todo: Adds to Server "iAmOwner" as in the measurables
module ShowIfSameUser = {
  [@react.component]
  let make = (~agentId, ~children) => {
    <Providers.AppContext.Consumer>
      ...{({loggedUser}) => {
        let isCorrect =
          loggedUser
          |> E.O.fmap((r: Types.user) => r.agentId == agentId)
          |> E.O.default(false);
        isCorrect ? children : ReasonReact.null;
      }}
    </Providers.AppContext.Consumer>;
  };
};

module Tabs = {
  open FC;

  [@react.component]
  let make = (~switchTab, ~tab, ~notebook: Types.notebook) => {
    <Div
      styles=[
        Css.style([
          Css.maxWidth(`px(682)),
          Css.marginLeft(`auto),
          Css.marginRight(`auto),
        ]),
      ]>
      <Div
        styles=[
          Css.style([
            FC.Base.BaseStyles.floatRight,
            Css.paddingTop(`em(0.2)),
          ]),
        ]>
        <TabButton isActive={tab == Show} onClick={_ => switchTab(Show)}>
          {"Notebook" |> Utils.ste}
        </TabButton>
        <TabButton
          isActive={tab == Details} onClick={_ => switchTab(Details)}>
          {"Markdown" |> Utils.ste}
        </TabButton>
        <ShowIfSameUser agentId={notebook.ownerId}>
          <TabButton isActive={tab == Edit} onClick={_ => switchTab(Edit)}>
            {"Edit" |> Utils.ste}
          </TabButton>
        </ShowIfSameUser>
      </Div>
    </Div>;
  };
};

[@react.component]
let make = (~notebookId: string) => {
  let (tab, setTab) = React.useState(() => Show);

  let notebookRedux = NotebookRedux.reducer();

  let switchTab = tabToSwitch => {
    setTab(_ => tabToSwitch);
    notebookRedux.dispatch(DeselectMeasurableId);
  };

  NotebookGet.component(~id=notebookId, notebook =>
    switch (notebook) {
    | Some(notebook) =>
      <Div flexDirection=`row styles=[Styles.full]>
        <Div flex={`num(5.)}>
          <SLayout isFluid=true>
            <Div flexDirection=`column>
              <Div flex={`num(1.)}> <Tabs switchTab tab notebook /> </Div>
              <Div flex={`num(1.)}>
                {switch (tab) {
                 | Show =>
                   <div
                     className=Css.(
                       style([
                         marginTop(`em(2.0)),
                         marginBottom(`em(2.0)),
                       ])
                     )>
                     <NotebookHeader notebook />
                     <NotebookMarkdown
                       blocks={NotebookMarkdown.markdownToBlocks(
                         notebook.body,
                       )}
                       notebookRedux
                     />
                   </div>
                 | Details =>
                   <FC__PageCard.BodyPadding>
                     <Antd.Input.TextArea
                       style={ReactDOMRe.Style.make(~minHeight="80em", ())}
                       value={notebook.body}
                     />
                   </FC__PageCard.BodyPadding>
                 | Edit =>
                   <NotebookUpdate
                     notebook
                     onSuccess={_ => switchTab(Show)}
                   />
                 }}
              </Div>
            </Div>
          </SLayout>
        </Div>
        {NotebookSidebar.make(~notebookRedux)
         |> E.O.React.fmapOrNull(sidebar =>
              <Div flex={`num(3.)} styles=[Styles.sidebarOutside]>
                <div className=Styles.sidebar> sidebar </div>
              </Div>
            )}
      </Div>

    | _ => <NotFoundPage />
    }
  );
};