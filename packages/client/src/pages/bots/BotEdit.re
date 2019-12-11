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
       | Some(bot) => <BotForm.Edit bot loggedUser />
       | _ => <NotFoundPage />
       }
     )}
  </SLayout>;
};