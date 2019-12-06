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

       let reform = NotebookForm.withForm(onSubmit, notebook);

       let form = NotebookForm.formFields(reform);

       let onSuccess = _ =>
         {Routing.Url.push(ChannelNotebooks(channelId))
          ReasonReact.null};

       <NotebookForm.Form.Provider value=reform>
         {CMutationForm.showWithLoading2(
            ~result=data.result,
            ~form,
            ~onSuccess,
            (),
          )}
       </NotebookForm.Form.Provider>;
     })}
  </SLayout>;
};