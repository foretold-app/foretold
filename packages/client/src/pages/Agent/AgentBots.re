open Utils;
open Foretold__GraphQL;
open Css;
open Style.Grid;

let block = style([marginBottom(`em(1.0))]);

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
    render: (r: Context.Primary.Bot.t) =>
      switch (r.name, r.agent) {
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
    render: (r: Context.Primary.Bot.t) =>
      r.description |> Rationale.Option.default("") |> ste,
    flex: 2,
  };

  let tokenColumn: column = {
    name: "Token" |> ste,
    render: (r: Context.Primary.Bot.t) =>
      <Antd.Input value={r.token |> Rationale.Option.default("")} />,
    flex: 2,
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