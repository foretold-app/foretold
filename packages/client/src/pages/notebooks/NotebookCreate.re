[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = NotebookCreateMutation.Query.t;
  });

[@react.component]
let make = (~loggedUser: Types.user) => {
  let body =
    NotebookCreateMutation.withMutation((mutation, data) => {
      let onSubmit = (values: NotebookForm.Form.onSubmitAPI): unit => {
        BotCreateMutation.mutate(
          mutation,
          values.state.values.name,
          values.state.values.description,
          values.state.values.competitorType,
          values.state.values.picture,
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

          CMutationForm.showWithLoading2(
            ~result=data.result,
            ~form,
            ~onSuccess=_ => NotebookForm.onSuccess(loggedUser, ()),
            (),
          );
        },
      );
    });

  <SLayout head={SLayout.Header.textDiv("Make a New Notebook")}>
    body
  </SLayout>;
};