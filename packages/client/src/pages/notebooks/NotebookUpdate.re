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

    NotebookForm.withForm(
      onSubmit,
      Some(notebook),
      ({handleChange, state, getFieldState, submit}) => {
        let form =
          NotebookForm.formFields(state, handleChange, getFieldState, submit);

        let onSuccess = e => {
          onSuccess(e);
          <Null />;
        };

        CMutationForm.showWithLoading2(
          ~result=data.result,
          ~form,
          ~onSuccess,
          (),
        );
      },
    );
  });
};