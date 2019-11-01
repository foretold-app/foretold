[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = NotebookCreateMutation.Query.t;
  });

[@react.component]
let make = () => {
  let body =
    NotebookCreateMutation.withMutation((mutation, data) => {
      let onSubmit = (values: NotebookForm.Form.onSubmitAPI): unit => {
        NotebookCreateMutation.mutate(
          mutation,
          values.state.values.name,
          values.state.values.description,
          values.state.values.body,
        );
        ();
      };

      let notebook = None;

      NotebookForm.withForm(
        onSubmit,
        notebook,
        ({send, state}) => {
          let form =
            NotebookForm.formFields(state, send, () =>
              send(NotebookForm.Form.Submit)
            );

          CMutationForm.showWithLoading(~result=data.result, ~form, ());
        },
      );
    });

  <SLayout head={SLayout.Header.textDiv("Make a New Notebook")}>
    body
  </SLayout>;
};