[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = BotCreateMutation.Query.t;
  });

[@react.component]
let make = (~loggedUser: Types.user) => {
  let body =
    BotCreateMutation.withMutation((mutation, data) => {
      let onSubmit = (values: BotForm.Form.onSubmitAPI) => {
        BotCreateMutation.mutate(
          mutation,
          values.state.values.name,
          values.state.values.description,
          values.state.values.competitorType,
          values.state.values.picture,
        );
        None;
      };

      let bot = None;

      BotForm.withForm(
        onSubmit,
        bot,
        ({handleChange, submit, state}: BotForm.Form.api) => {
          let form =
            BotForm.formFields(state, handleChange, () =>
              submit(BotForm.Form.Submit)
            );

          CMutationForm.showWithLoading2(
            ~result=data.result,
            ~form,
            ~onSuccess=_ => BotForm.onSuccess(loggedUser, ()),
            (),
          );
        },
      );
    });

  <SLayout head={SLayout.Header.textDiv("Make a New Bot")}> body </SLayout>;
};