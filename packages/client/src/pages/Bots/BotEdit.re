open Foretold__GraphQL;

let component = ReasonReact.statelessComponent("BotEdit");

let make =
    (
      ~pageParams: PageConfig.LoggedInPage.pageParams,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let body =
      Mutations.BotCreate.withMutation((mutation, data) =>
        BotForm.withForm(mutation, ({send, state}) =>
          BotForm.CMutationForm.showWithLoading(
            ~result=data.result,
            ~form=
              BotForm.formFields(state, send, () =>
                send(BotForm.Form.Submit)
              ),
            (),
          )
        )
      );

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Make a New Bot"),
      ~body,
    )
    |> layout;
  },
};