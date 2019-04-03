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
    let columns = [|
      Antd.Table.TableProps.make_column(
        ~title="Channel",
        ~dataIndex="agent",
        ~key="channelName",
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
        ~title="Channel",
        ~dataIndex="role",
        ~key="channelName",
        ~width=2,
        ~render=
          (~text, ~record, ~index) =>
            switch (record##role) {
            | "VIEWER" =>
              <div className="ant-tag ant-tag-green"> {"Viewer" |> ste} </div>
            | "ADMIN" =>
              <div className="ant-tag ant-tag-blue"> {"ADMIN" |> ste} </div>
            | _ => <div />
            },
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
             <Antd.Table columns dataSource />;
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