open Style.Grid;

module Styles = {
  open Css;
  let full = style([Css.float(`left), width(`percent(100.))]);
  let sidebarOutside = style([background(`hex("f0f2f5"))]);
  let sidebar =
    style([
      position(`sticky),
      top(`em(0.0)),
      Css.float(`left),
      width(`percent(100.0)),
    ]);
  let sidebarInner =
    style([
      width(`percent(98.0)),
      float(`left),
      overflow(`scroll),
      height(`vh(99.0)),
    ]);
};

type tab =
  | Show
  | Edit
  | Details;

module Tabs = {
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
            ForetoldComponents.Base.BaseStyles.floatRight,
            Css.paddingTop(`em(0.2)),
          ]),
        ]>
        <ForetoldComponents.Tab.Button
          isActive={tab == Show} onClick={_ => switchTab(Show)}>
          {"Notebook" |> Utils.ste}
        </ForetoldComponents.Tab.Button>
        <ForetoldComponents.Tab.Button
          isActive={tab == Details} onClick={_ => switchTab(Details)}>
          {"Markdown" |> Utils.ste}
        </ForetoldComponents.Tab.Button>
        {<ForetoldComponents.Tab.Button
           isActive={tab == Edit} onClick={_ => switchTab(Edit)}>
           {"Edit" |> Utils.ste}
         </ForetoldComponents.Tab.Button>
         |> E.React2.showIf(
              Primary.Permissions.can(`NOTEBOOK_UPDATE, notebook.permissions),
            )}
        {<Bookmarks.BookmarkNotebook notebook />
         |> E.React2.showIf(
              Primary.Permissions.can(
                `NOTEBOOK_BOOKMARK_TOGGLE,
                notebook.permissions,
              ),
            )}
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
          <SLayout container=`fluid>
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
                   <ForetoldComponents.PageCard.BodyPadding>
                     <Antd.Input.TextArea
                       style={ReactDOMRe.Style.make(~minHeight="80em", ())}
                       value={notebook.body}
                     />
                   </ForetoldComponents.PageCard.BodyPadding>
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
                <div className=Styles.sidebar>
                  <div className=Styles.sidebarInner> sidebar </div>
                </div>
              </Div>
            )}
      </Div>

    | _ => <NotFoundPage />
    }
  );
};