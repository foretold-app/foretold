module CMutationForm =
  MutationForm.Make({
    type queryType = BotCreateMutation.Query.t;
  });

[@react.component]
let make = (~loggedUser: Types.user) => {
  <SLayout head={SLayout.Header.textDiv("Make a New Bot")}>
    <Providers.AppContext.Consumer>
      ...{({loggedUser}) =>
        switch (loggedUser) {
        | Some(loggedUser) => <BotForm.Create loggedUser />
        | _ => <Null />
        }
      }
    </Providers.AppContext.Consumer>
  </SLayout>;
};