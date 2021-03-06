open Utils;
open Style.Grid;

module Styles = {
  open Css;
  let paddingRight = [paddingRight(`em(1.))] |> style;
};

module Title = {
  [@react.component]
  let make = () =>
    <ForetoldComponents.Base.Div float=`left>
      <ForetoldComponents.PageCard.HeaderRow.Title>
        {"Bots" |> Utils.ste}
      </ForetoldComponents.PageCard.HeaderRow.Title>
    </ForetoldComponents.Base.Div>;
};

module AgentSection = {
  [@react.component]
  let make = (~agent: Types.agent) =>
    switch (agent.agentType) {
    | Some(User(_)) =>
      <>
        {E.React2.showIf(
           agent.isMe,
           <Div float=`right>
             <Antd.Button
               onClick={_ => Routing.Url.push(BotCreate)} _type=`primary>
               {"New Bot" |> ste}
             </Antd.Button>
           </Div>,
         )}
      </>
    | _ => E.React2.null
    };
};

module Columns = {
  type column = Table.column(Types.bot);

  let name =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (bot: Types.bot) =>
          switch (bot.name, bot.agent) {
          | (Some(_), Some(agent)) =>
            <Link
              linkType={
                Internal(Agent({agentId: agent.id, subPage: AgentUpdates}))
              }>
              <AgentLink agent />
            </Link>
          | _ => <Null />
          },
      (),
    );

  let description =
    Table.Column.make(
      ~name="Description" |> ste,
      ~render=
        (bot: Types.bot) =>
          bot.description |> Rationale.Option.default("") |> ste,
      (),
    );

  let token =
    Table.Column.make(
      ~name="Token" |> ste,
      ~render=
        (bot: Types.bot) =>
          <div className=Styles.paddingRight>
            <Antd.Input value={bot.token |> Rationale.Option.default("")} />
          </div>,
      ~show=(bot: Types.bot) => bot.token != None,
      (),
    );

  let edit =
    Table.Column.make(
      ~name="Edit" |> ste,
      ~render=
        (bot: Types.bot) =>
          <Link linkType={Internal(BotEdit(bot.id))}>
            {"Edit Bot" |> ste}
          </Link>,
      ~show=
        (bot: Types.bot) =>
          Primary.Permissions.can(`BOT_UPDATE, bot.permissions),
      (),
    );

  let all = [|name, description, token, edit|];
};

[@react.component]
let make = (~pageParams: Types.pageParams) => {
  AgentGet.component(~id=pageParams.id, agent =>
    switch (agent) {
    | Success(Some(agent)) =>
      let showBots = bots =>
        Array.length(bots) > 0
          ? <ForetoldComponents.PageCard.Body>
              <Table columns=Columns.all rows=bots />
            </ForetoldComponents.PageCard.Body>
          : <NothingToShow />;

      let body =
        switch (agent.agentType) {
        | Some(User(user)) => BotsGet.component(~ownerId=user.id, showBots)
        | _ => <NothingToShow />
        };

      let head =
        <div>
          <Title />
          <ForetoldComponents.Base.Div
            float=`right
            className={Css.style([
              ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
              ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
            ])}>
            <AgentSection agent />
          </ForetoldComponents.Base.Div>
        </div>;

      <SLayout head> body </SLayout>;
    | _ => <SLayout> <NothingToShow /> </SLayout>
    }
  );
};