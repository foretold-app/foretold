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
      {SLayout.Header.textDiv(r.name ++ ": Bots")}
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
        <Foretold__Components__Link linkType={Internal(AgentShow(agent.id))}>
          {name |> ste}
        </Foretold__Components__Link>
      | _ => ReasonReact.null
      },
  };
  let descriptionColumn: column = {
    name: "Description" |> ste,
    render: (r: record) => r.description |> E.O.default("") |> ste,
  };

  let tokenColumn: column = {
    name: "Token" |> ste,
    render: (r: record) => <Antd.Input value={r.jwt |> E.O.default("")} />,
  };

  let all = [|nameColumn, descriptionColumn, tokenColumn|];
};

let make = (~id: string, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    Queries.Agent.component(~id, ({agent}) =>
      Queries.Bots.component(bots =>
        SLayout.LayoutConfig.make(
          ~head=agentSection(agent),
          ~body=Table.fromColumns(Columns.all, bots),
        )
        |> layout
      )
    ),
};