module CMutationForm =
  MutationForm.Make({
    type queryType = BotCreateMutation.Query.t;
  });

let component = ReasonReact.statelessComponent("BotCreate");
let make = (~loggedUser: Types.user, _children) => {
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
              BotForm.formFields(state, send, () =>
                send(BotForm.Form.Submit)
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
  },
};