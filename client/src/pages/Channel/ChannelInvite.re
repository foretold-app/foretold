open Utils;
open E;
open Css;
open SLayout;
open Foretold__GraphQL;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("ChannelMembers");

let make =
    (
      ~channelId: string,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let addToChannel = (agentId, channelId) =>
      Foretold__GraphQL.Mutations.ChannelMembershipCreate.Mutation.make(
        (mutation, _) =>
        <a
          href=""
          onClick={
            _ =>
              Foretold__GraphQL.Mutations.ChannelMembershipCreate.mutate(
                mutation,
                agentId,
                channelId,
              )
          }>
          {"Add to Channel" |> ste}
        </a>
      )
      |> E.React.el;

    let columns = [|
      Antd.Table.TableProps.make_column(
        ~title="Agent",
        ~dataIndex="agent",
        ~key="agent",
        ~width=2,
        ~render=
          (~text, ~record, ~index) =>
            <a
              onClick={
                _ => Context.Routing.Url.push(AgentShow(record##agentId))
              }
              href="">
              {record##agentName |> ste}
            </a>,
        (),
      ),
      Antd.Table.TableProps.make_column(
        ~title="Remove",
        ~dataIndex="role",
        ~key="actions2",
        ~width=2,
        ~render=
          (~text, ~record, ~index) =>
            addToChannel(record##agentId, channelId),
        (),
      ),
    |];

    let table =
      Foretold__GraphQL.Queries.Agents.component(agents =>
        agents
        |> E.HttpResponse.fmap(agents => {
             let dataSource =
               agents
               |> E.A.fmap((r: Context.Primary.Agent.t) =>
                    {
                      "key": r.id,
                      "agentId": r.id,
                      "agentName": r.name |> E.O.default(""),
                    }
                  );
             <Antd.Table columns dataSource size=`small />;
           })
        |> E.HttpResponse.withReactDefaults
      );

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Invite Agents"),
      ~body=table,
    )
    |> layout;
  },
};