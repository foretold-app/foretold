[@bs.config {jsx: 3}];

[@react.component]
let make = (~notebookPage: Routing.NotebookPage.t) => {
  <FillWithSidebar>
    {switch (notebookPage.subPage) {
     | _ => <NotebookPage notebookPage />
     }}
  </FillWithSidebar>;
};