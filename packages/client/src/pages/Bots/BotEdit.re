open Foretold__GraphQL;

let component = ReasonReact.statelessComponent("BotEdit");

module CMutationForm =
  MutationForm.Make({
    type queryType = Mutations.BotUpdate.Query.t;
  });

let make =
    (
      ~pageParams: PageConfig.LoggedInPage.pageParams,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let getForm = (bot: option(Context.Primary.Bot.t)) =>
      Mutations.BotUpdate.withMutation((mutation, data) => {
        let onSubmit = (values: BotForm.Form.onSubmitAPI): unit => {
          Mutations.BotUpdate.mutate(
            mutation,
            pageParams.id,
            values.state.values.name,
            values.state.values.description,
            values.state.values.competitorType,
          );
          ();
        };

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
              ~onSuccess=BotForm.onSuccess(loggedInUser),
              (),
            );
          },
        );
      });

    let botId = pageParams.id;
    let body = Queries.Bot.component(~id=botId, getForm);

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Edit a Bot"),
      ~body,
    )
    |> layout;
  },
};