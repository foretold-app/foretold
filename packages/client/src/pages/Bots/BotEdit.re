open Foretold__GraphQL;

let component = ReasonReact.statelessComponent("BotEdit");

module CMutationForm =
  MutationForm.Make({
    type queryType = Mutations.BotUpdate.Query.t;
  });

let make =
    (
      ~pageParams: PageConfig.LoggedInPage.pageParams,
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
              ~onSuccess=
                () => {
                  Context.Routing.Url.push(
                    Agent({
                      agentId: "14810584-6ea2-48ab-9d85-3bb839b2f661",
                      subPage: AgentBots,
                    }),
                  );
                  ReasonReact.null;
                },
              (),
            );
          },
        );
      });

    let body = Queries.Bot.component(~id=pageParams.id, bot => getForm(bot));

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Edit a Bot"),
      ~body,
    )
    |> layout;
  },
};