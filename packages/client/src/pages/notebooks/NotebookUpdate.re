[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = NotebookUpdateMutation.Query.t;
  });

[@react.component]
let make = (~notebook: Types.notebook, ~onSuccess) => {
  NotebookUpdateMutation.withMutation((mutation, data) => {
    let onSubmit = (values: NotebookForm.Form.onSubmitAPI) => {
      NotebookUpdateMutation.mutate(
        mutation,
        notebook.id,
        values.state.values.name,
        values.state.values.body,
      );
      None;
    };

    let reform = NotebookForm.withForm(onSubmit, Some(notebook));

    let form = NotebookForm.formFields(reform);

    let onSuccess = e => {
      onSuccess(e);
      <Null />;
    };

    <NotebookForm.Form.Provider value=reform>
      {CMutationForm.showWithLoading2(
         ~result=data.result,
         ~form,
         ~onSuccess,
         (),
       )}
    </NotebookForm.Form.Provider>;
  });
};