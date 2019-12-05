[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = NotebookCreateMutation.Query.t;
  });

[@react.component]
let make = (~channelId: string) => {
  <SLayout head={SLayout.Header.textDiv("Make a New Notebook")} isFluid=true>
    {NotebookCreateMutation.withMutation((mutation, data) => {
       let onSubmit = (values: NotebookForm.Form.onSubmitAPI) =>
         {NotebookCreateMutation.mutate(
            mutation,
            channelId,
            values.state.values.name,
            values.state.values.body,
          )
          None};

       let notebook = None;

       NotebookForm.withForm(
         onSubmit,
         notebook,
         (
           {handleChange, state, getFieldState, submit}: NotebookForm.Form.api,
         ) => {
           let form =
             NotebookForm.formFields(
               state,
               handleChange,
               getFieldState,
               submit,
             );

           let onSuccess = _ =>
             {Routing.Url.push(ChannelNotebooks(channelId))
              ReasonReact.null};

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