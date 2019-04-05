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
    let changeRoleAction = (agentId, channelId, role, text) =>
      Foretold__GraphQL.Mutations.ChannelMembershipRoleUpdate.Mutation.make(
        (mutation, _) =>
        <a
          href=""
          onClick={
            _ =>
              Foretold__GraphQL.Mutations.ChannelMembershipRoleUpdate.mutate(
                mutation,
                ~agentId,
                ~channelId,
                ~role,
              )
          }>
          {text |> ste}
        </a>
      )
      |> E.React.el;

    let removeFromChannel = (agentId, channelId) =>
      Foretold__GraphQL.Mutations.ChannelLeave.Mutation.make((mutation, _) =>
        <a
          href=""
          onClick={
            _ =>
              Foretold__GraphQL.Mutations.ChannelLeave.mutate(
                mutation,
                agentId,
                channelId,
              )
          }>
          {"Remove" |> ste}
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
        ~title="Role",
        ~dataIndex="role",
        ~key="role",
        ~width=2,
        ~render=
          (~text, ~record, ~index) =>
            switch (record##role) {
            | "Viewer" =>
              <div className="ant-tag ant-tag-green"> {"Viewer" |> ste} </div>
            | "Admin" =>
              <div className="ant-tag ant-tag-blue"> {"Admin" |> ste} </div>
            | _ => <div />
            },
        (),
      ),
      Antd.Table.TableProps.make_column(
        ~title="Change Role",
        ~dataIndex="role",
        ~key="actions",
        ~width=2,
        ~render=
          (~text, ~record, ~index) =>
            switch (record##role) {
            | "Viewer" =>
              changeRoleAction(
                record##agentId,
                channelId,
                `ADMIN,
                "Change to Admin",
              )
            | "Admin" =>
              changeRoleAction(
                record##agentId,
                channelId,
                `VIEWER,
                "Change to Viewer",
              )
            | _ => <div />
            },
        (),
      ),
      Antd.Table.TableProps.make_column(
        ~title="Remove",
        ~dataIndex="role",
        ~key="actions2",
        ~width=2,
        ~render=
          (~text, ~record, ~index) =>
            removeFromChannel(record##agentId, channelId),
        (),
      ),
    |];

    let table =
      Queries.ChannelMemberships.component(~id=channelId, memberships =>
        memberships
        |> E.HttpResponse.fmap(memberships => {
             let dataSource =
               memberships
               |> E.A.fmap((r: Context.Primary.Types.channelMembership) =>
                    {
                      "key":
                        r.agent
                        |> E.O.fmap((r: Context.Primary.Types.agent) => r.id)
                        |> E.O.default(""),
                      "agentName":
                        r.agent |> E.O.bind(_, r => r.name) |> E.O.default(""),
                      "agentId":
                        r.agent
                        |> E.O.fmap((r: Context.Primary.Types.agent) => r.id)
                        |> E.O.default(""),
                      "role":
                        r.role
                        |> Context.Primary.ChannelMembershipRole.toString,
                    }
                  );
             <Antd.Table columns dataSource size=`small />;
           })
        |> E.HttpResponse.withReactDefaults
      );

    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Channel Members"),
      ~body=table,
    )
    |> layout;
  },
};