[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = BotCreateMutation.Query.t;
  });

[@react.component]
let make = (~loggedUser: Types.user) => {
  //  let body =
  //    BotCreateMutation.withMutation((mutation, data) => {
  //      let onSubmit = (values: BotForm.Form.onSubmitAPI) => {
  //        BotCreateMutation.mutate(
  //          mutation,
  //          values.state.values.name,
  //          values.state.values.description,
  //          values.state.values.competitorType,
  //          values.state.values.picture,
  //        );
  //        None;
  //      };
  //
  //      let bot = None;
  //
  //      let reform = BotForm.withForm(onSubmit, bot);
  //      let form = BotForm.formFields(reform);
  //
  //      <BotForm.Form.Provider value=reform>
  //        {CMutationForm.showWithLoading2(
  //           ~result=data.result,
  //           ~form,
  //           ~onSuccess=_ => BotForm.onSuccess(loggedUser, ()),
  //           (),
  //         )}
  //    });
  <SLayout head={SLayout.Header.textDiv("Make a New Bot")}>
    //      </BotForm.Form.Provider>;
     <BotCreateMutation2 /> </SLayout>;
};