open Utils;
open Foretold__GraphQL;
open Css;
open Style.Grid;

let block = style([marginBottom(`em(1.0))]);
let notFound = "Agent not found" |> ste |> E.React.inH3;

let agentSection = (e: Queries.Agent.agent) =>
  switch (e) {
  | {user: Some(r)} =>
    <>
      {
        E.React.showIf(
          e.isMe,
          <Div float=`right>
            <Antd.Button
              onClick={_ => Context.Routing.Url.push(BotCreate)}
              _type=`primary>
              {"New Bot" |> ste}
            </Antd.Button>
          </Div>,
        )
      }
    </>
  | _ => notFound
  };

let component = ReasonReact.statelessComponent("AgentBots");

module Columns = {
  type record = Context.Primary.Bot.t;
  type column = Table.column(Context.Primary.Bot.t);
  let nameColumn: column = {
    name: "Name" |> ste,
    render: (r: record) =>
      switch (r.name, r.agent) {
      | (Some(name), Some(agent)) =>
        <Foretold__Components__Link
          linkType={Internal(Agent({agentId: agent.id, subPage: AgentShow}))}>
          {name |> ste}
        </Foretold__Components__Link>
      | _ => ReasonReact.null
      },
    flex: 1,
  };
  let descriptionColumn: column = {
    name: "Description" |> ste,
    render: (r: record) => r.description |> E.O.default("") |> ste,
    flex: 2,
  };

  let tokenColumn: column = {
    name: "Token" |> ste,
    render: (r: record) => <Antd.Input value={r.token |> E.O.default("")} />,
    flex: 2,
  };

  let all = [|nameColumn, descriptionColumn, tokenColumn|];
};

type pageParams = {id: string};
let make = (~pageParams, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    Queries.Agent.component(~id=pageParams.id, ({agent}) =>
      Queries.Bots.component(bots =>
        SLayout.LayoutConfig.make(
          ~head=agentSection(agent),
          ~body=
            <FC.PageCard.BodyPadding>
              {Table.fromColumns(Columns.all, bots)}
            </FC.PageCard.BodyPadding>,
        )
        |> layout
      )
    ),
};