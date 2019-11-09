[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = NotebookUpdateMutation.Query.t;
  });

[@react.component]
let make = (~notebook: Types.notebook) => {
  NotebookUpdateMutation.withMutation((mutation, data) => {
    let onSubmit = (values: NotebookForm.Form.onSubmitAPI): unit =>
      NotebookUpdateMutation.mutate(
        mutation,
        notebook.id,
        values.state.values.name,
        values.state.values.body,
      );

    NotebookForm.withForm(
      onSubmit,
      Some(notebook),
      ({send, state, getFieldState}) => {
        let form = NotebookForm.formFields(state, send, getFieldState);

        let onSuccess = _ => {
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