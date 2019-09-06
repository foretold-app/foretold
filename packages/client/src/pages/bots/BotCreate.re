let component = ReasonReact.statelessComponent("BotCreate");

module CMutationForm =
  MutationForm.Make({
    type queryType = BotCreateMutation.Query.t;
  });

let make =
    (
      ~loggedInUser: Types.user,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let body =
      BotCreateMutation.withMutation((mutation, data) => {
        let onSubmit = (values: BotForm.Form.onSubmitAPI): unit => {
          BotCreateMutation.mutate(
            mutation,
            values.state.values.name,
            values.state.values.description,
            values.state.values.competitorType,
          );
          ();
        };

        let bot = None;

        BotForm.withForm(
          onSubmit,
          bot,
          ({send, state}) => {
            let form =
              BotForm.formFields(state, send, () =>
                send(BotForm.Form.Submit)
              );

            CMutationForm.showWithLoading2(
              ~result=data.result,
              ~form,
              ~onSuccess=_ => BotForm.onSuccess(loggedInUser, ()),
              (),
            );
          },
        );
      });

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Make a New Bot"),
      ~body,
    )
    |> layout;
  },
};