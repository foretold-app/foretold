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

let columns = [|
  Antd.Table.TableProps.make_column(
    ~title="Bot",
    ~dataIndex="name",
    ~key="name",
    ~width=2,
    ~render=
      (~text, ~record, ~index) =>
        <a
          onClick={_ => Context.Routing.Url.push(AgentShow(record##agentId))}
          href="">
          {record##name |> E.O.default("") |> ste}
        </a>,
    (),
  ),
  Antd.Table.TableProps.make_column(
    ~title="Description",
    ~dataIndex="description",
    ~key="name",
    ~width=2,
    ~render=
      (~text, ~record, ~index) =>
        record##description |> E.O.default("") |> ste,
    (),
  ),
  Antd.Table.TableProps.make_column(
    ~title="Bot Type",
    ~dataIndex="competitorType",
    ~key="competitorType",
    ~width=2,
    ~render=
      (~text, ~record, ~index) =>
        record##competitorType
        |> Context.Primary.Bot.CompetitorType.toString
        |> ste,
    (),
  ),
  Antd.Table.TableProps.make_column(
    ~title="Token",
    ~dataIndex="jwt",
    ~key="name",
    ~width=2,
    ~render=
      (~text, ~record, ~index) =>
        <Antd.Input value={record##jwt |> E.O.default("")} />,
    (),
  ),
|];

let make = (~id: string, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    Queries.Agent.component(~id, ({agent}) =>
      Queries.Bots.component(bots =>
        SLayout.LayoutConfig.make(
          ~head=agentSection(agent),
          ~body=
            <div>
              {
                let dataSource =
                  bots
                  |> E.A.fmap((b: Context.Primary.Bot.t) =>
                       {
                         "key": b.id,
                         "description": b.description,
                         "agentId":
                           b.agent
                           |> E.O.fmap((r: Context.Primary.Agent.t) => r.id)
                           |> E.O.default(""),
                         "name": b.name,
                         "jwt": b.jwt,
                         "competitorType": b.competitorType,
                       }
                     );
                <Antd.Table columns dataSource size=`small />;
              }
            </div>,
        )
        |> layout
      )
    ),
};