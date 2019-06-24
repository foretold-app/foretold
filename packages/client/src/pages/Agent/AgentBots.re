open Utils;
open Foretold__GraphQL;
open Style.Grid;

module Styles = {
  open Css;
  let paddingRight = [paddingRight(`em(1.))] |> style;
};

let agentSection = (agent: Queries.Agent.agent) =>
  switch (agent) {
  | {user: Some(_user)} =>
    <>
      {E.React.showIf(
         agent.isMe,
         <Div float=`right>
           <Antd.Button
             onClick={_ => Context.Routing.Url.push(BotCreate)}
             _type=`primary>
             {"New Bot" |> ste}
           </Antd.Button>
         </Div>,
       )}
    </>
  | _ => E.React.null
  };

let component = ReasonReact.statelessComponent("AgentBots");

module Columns = {
  type column = Table.column(Context.Primary.Bot.t);

  let nameColumn: column = {
    name: "Name" |> ste,
    render: (bot: Context.Primary.Bot.t) =>
      switch (bot.name, bot.agent) {
      | (Some(name), Some(agent)) =>
        <Foretold__Components__Link
          linkType={
            Internal(Agent({agentId: agent.id, subPage: AgentMeasurements}))
          }>
          {name |> ste}
        </Foretold__Components__Link>
      | _ => ReasonReact.null
      },
    flex: 1,
  };

  let descriptionColumn: column = {
    name: "Description" |> ste,
    render: (bot: Context.Primary.Bot.t) =>
      bot.description |> Rationale.Option.default("") |> ste,
    flex: 1,
  };

  let tokenColumn: column = {
    name: "Token" |> ste,
    render: (bot: Context.Primary.Bot.t) =>
      <div className=Styles.paddingRight>
        <Antd.Input value={bot.token |> Rationale.Option.default("")} />
      </div>,
    flex: 1,
  };

  let editColumn: column = {
    name: "Edit" |> ste,
    render: (bot: Context.Primary.Bot.t) =>
      <Foretold__Components__Link linkType={Internal(BotEdit(bot.id))}>
        {"Edit Bot" |> ste}
      </Foretold__Components__Link>,

    flex: 1,
  };

  let all: array(column) = [|
    nameColumn,
    descriptionColumn,
    tokenColumn,
    editColumn,
  |];
};

type pageParams = {id: string};

let getUserId = (agent: Queries.Agent.agent) => {
  switch (agent.user) {
  | Some(user) => user.id
  | None => ""
  };
};

let make = (~pageParams, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    Queries.Agent.component(~id=pageParams.id, ({agent}) =>
      Queries.Bots.component(~ownerId=getUserId(agent), bots =>
        SLayout.LayoutConfig.make(
          ~head=
            <FC.Base.Div
              float=`right
              className={Css.style([
                FC.PageCard.HeaderRow.Styles.itemTopPadding,
                FC.PageCard.HeaderRow.Styles.itemBottomPadding,
              ])}>
              {agentSection(agent)}
            </FC.Base.Div>,
          ~body=
            <FC.PageCard.Body>
              {Table.fromColumns(Columns.all, bots)}
            </FC.PageCard.Body>,
        )
        |> layout
      )
    ),
};