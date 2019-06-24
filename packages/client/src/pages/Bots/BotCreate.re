open Foretold__GraphQL;

let component = ReasonReact.statelessComponent("BotCreates");

module CMutationForm =
  MutationForm.Make({
    type queryType = Mutations.BotCreate.Query.t;
  });

let make = (~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let body =
      Mutations.BotCreate.withMutation((mutation, data) => {
        let onSubmit = (values: BotForm.Form.onSubmitAPI): unit => {
          Mutations.BotCreate.mutate(
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

            CMutationForm.showWithLoading(~result=data.result, ~form, ());
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