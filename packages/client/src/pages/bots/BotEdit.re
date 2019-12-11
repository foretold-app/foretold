module CMutationForm =
  MutationForm.Make({
    type queryType = BotUpdate.Query.t;
  });

[@react.component]
let make = (~pageParams: Types.pageParams, ~loggedUser: Types.user) => {
  let botId = pageParams.id;

  <SLayout head={SLayout.Header.textDiv("Edit a Bot")}>
    {BotGet.component(~id=botId, (bot: option(Types.bot)) =>
       switch (bot) {
       | Some(bot) =>
         <Providers.AppContext.Consumer>
           ...{({loggedUser}) =>
             switch (loggedUser) {
             | Some(loggedUser) => <BotForm.Edit bot loggedUser />
             | _ => <Null />
             }
           }
         </Providers.AppContext.Consumer>
       | _ => <NotFoundPage />
       }
     )}
  </SLayout>;
};