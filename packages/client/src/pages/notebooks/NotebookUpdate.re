[@bs.config {jsx: 3}];

[@react.component]
let make = (~notebook: Types.notebook, ~onSuccess) => {
  <NotebookForm.Edit notebook />;
};