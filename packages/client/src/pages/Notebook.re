[@bs.config {jsx: 3}];

open Style.Grid;

[@react.component]
let make = (~notebookId: Types.notebookId) => {
  <SLayout head={"head" |> Utils.ste} isFluid=true>
    {"body" |> Utils.ste}
  </SLayout>;
};
