[@bs.config {jsx: 3}];

open Style.Grid;

[@react.component]
let make = (~notebookPage: Routing.NotebookPage.t) => {
  <SLayout head={"head" |> Utils.ste} isFluid=true>
    {"body" |> Utils.ste}
  </SLayout>;
};