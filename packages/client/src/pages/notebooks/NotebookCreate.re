[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = NotebookCreateMutation.Query.t;
  });

[@react.component]
let make = (~channelId: string) => {
  <SLayout head={SLayout.Header.textDiv("Make a New Notebook")}>
    {NotebookCreateMutation.withMutation((mutation, data) => {
       let onSubmit = (values: NotebookForm.Form.onSubmitAPI): unit =>
         NotebookCreateMutation.mutate(
           mutation,
           channelId,
           values.state.values.name,
           values.state.values.body,
         );

       let notebook = None;

       NotebookForm.withForm(
         onSubmit,
         notebook,
         ({send, state, getFieldState}) => {
           let form =
             NotebookForm.formFields(state, send, getFieldState, () =>
               send(NotebookForm.Form.Submit)
             );

           let onSuccess = _ => {
             Routing.Url.push(ChannelNotebooks(channelId));
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
     })}
  </SLayout>;
};
