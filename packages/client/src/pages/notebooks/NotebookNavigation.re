[@bs.config {jsx: 3}];

open Style.Grid;

module Top = {
  [@react.component]
  let make = (~notebookPage: Routing.NotebookPage.t) => {
    <FC.GroupHeader>
      <Div flexDirection=`column>
        <Div
          flex={`num(1.)}
          styles=[
            Css.style([
              Css.fontSize(`em(1.2)),
              Css.marginBottom(`em(0.1)),
            ]),
          ]>
          {"Title" |> Utils.ste}
        </Div>
        <Div flex={`num(1.)} styles=[SLayout.Styles.descriptionText]>
          {"Description" |> Utils.ste}
        </Div>
      </Div>
    </FC.GroupHeader>;
  };
};

[@react.component]
let make = (~notebookPage: Routing.NotebookPage.t) => {
  <FillWithSidebar>
    <Top notebookPage />
    {switch (notebookPage.subPage) {
     | _ => <NotebookPage notebookPage />
     }}
  </FillWithSidebar>;
};