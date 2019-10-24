[@bs.config {jsx: 3}];

module CMutationForm =
  MutationForm.Make({
    type queryType = BotCreateMutation.Query.t;
  });

[@react.component]
let make = (~loggedUser: Types.user) => {
  let body =
    BotCreateMutation.withMutation((mutation, data) => {
      let onSubmit = (values: BotForm.Form.onSubmitAPI): unit => {
        BotCreateMutation.mutate(
          mutation,
          values.state.values.name,
          values.state.values.description,
          values.state.values.competitorType,
          values.state.values.picture,
        );
        ();
      };

      let bot = None;

      BotForm.withForm(
        onSubmit,
        bot,
        ({send, state}) => {
          let form =
            BotForm.formFields(state, send, () => send(BotForm.Form.Submit));

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
