open Foretold__GraphQL;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("ChannelMembers");

let changeRoleAction = (agentId, channelId, role, text) =>
  Foretold__GraphQL.Mutations.ChannelMembershipRoleUpdate.Mutation.make(
    (mutation, _) =>
    <Foretold__Components__Link
      linkType={
        Action(
          _ =>
            Foretold__GraphQL.Mutations.ChannelMembershipRoleUpdate.mutate(
              mutation,
              ~agentId,
              ~channelId,
              ~role,
            ),
        )
      }>
      {text |> ste}
    </Foretold__Components__Link>
  )
  |> E.React.el;

let removeFromChannel = (agentId, channelId) =>
  Foretold__GraphQL.Mutations.ChannelMembershipDelete.Mutation.make(
    (mutation, _) =>
    <Foretold__Components__Link
      linkType={
        Action(
          _ =>
            Foretold__GraphQL.Mutations.ChannelMembershipDelete.mutate(
              mutation,
              agentId,
              channelId,
            ),
        )
      }>
      {"Remove" |> ste}
    </Foretold__Components__Link>
  )
  |> E.React.el;

module Columns = {
  type column = Table.column(Context.Primary.Types.channelMembership);

  let canX =
      (
        permission: Context.Primary.permission,
        record: Context.Primary.Types.channelMembership,
      ) =>
    record.permissions
    |> E.O.fmap((permissions: Context.Primary.Permissions.t) =>
         Context.Primary.Permissions.canX(permission, permissions)
       )
    |> E.O.default(false);

  let agentColumn: column = {
    name: "Agent" |> ste,
    render: membership =>
      membership.agent
      |> E.O.fmap((r: Context.Primary.Types.agent) =>
           <Foretold__Components__Link
             linkType={Internal(Agent({agentId: r.id, subPage: AgentShow}))}>
             {r.name |> E.O.default("Anonymous") |> ste}
           </Foretold__Components__Link>
         )
      |> E.O.React.defaultNull,
    flex: 1,
  };

  let roleColumn: column = {
    name: "Role" |> ste,
    render: membership =>
      switch (membership.role) {
      | `ADMIN =>
        <div className="ant-tag ant-tag-blue"> {"Admin" |> ste} </div>
      | `VIEWER =>
        <div className="ant-tag ant-tag-green"> {"Viewer" |> ste} </div>
      },
    flex: 1,
  };

  let roleChangeColumn: string => column =
    channelId => {
      name: "Change Role" |> ste,
      render: membership =>
        <div>
          {switch (membership.role, membership.agent) {
           | (`VIEWER, Some(agent)) =>
             E.React.showIf(
               canX(`CHANNEL_MEMBERSHIP_ROLE_UPDATE, membership),
               changeRoleAction(
                 agent.id,
                 channelId,
                 `ADMIN,
                 "Change to Admin",
               ),
             )
           | (`ADMIN, Some(agent)) =>
             E.React.showIf(
               canX(`CHANNEL_MEMBERSHIP_ROLE_UPDATE, membership),
               changeRoleAction(
                 agent.id,
                 channelId,
                 `VIEWER,
                 "Change to Viewer",
               ),
             )
           | _ => <div />
           }}
        </div>,
      flex: 1,
    };

  let removeFromChannelColumn: string => column =
    channelId => {
      name: "Remove" |> ste,
      render: membership =>
        switch (
          membership.agent,
          canX(`CHANNEL_MEMBERSHIP_DELETE, membership),
        ) {
        | (Some(agent), true) => removeFromChannel(agent.id, channelId)
        | _ => ReasonReact.null
        },
      flex: 1,
    };

  let all = (channelId, channel: Context.Primary.Types.channel) => {
    switch (channel.myRole) {
    | Some(`ADMIN) => [|
        agentColumn,
        roleColumn,
        roleChangeColumn(channelId),
        removeFromChannelColumn(channelId),
      |]
    | _ => [|agentColumn, roleColumn|]
    };
  };
};

let make =
    (~channelId: string, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let load2Queries = (channelId, fn) =>
      ((a, b) => (a, b) |> fn)
      |> E.F.flatten2Callbacks(
           Queries.Channel.component2(~id=channelId),
           Queries.ChannelMemberships.component(~id=channelId),
         );

    let table =
      load2Queries(channelId, ((channel, memberships)) =>
        memberships
        |> E.HttpResponse.fmap(memberships =>
             Table.fromColumns(Columns.all(channelId, channel), memberships)
           )
        |> E.HttpResponse.withReactDefaults
      );

    SLayout.LayoutConfig.make(
      ~head=
        <>
          <FC.Base.Div float=`left>
            <FC.PageCard.HeaderRow.Title>
              {"Channel Members" |> ste}
            </FC.PageCard.HeaderRow.Title>
          </FC.Base.Div>
          <FC.Base.Div
            float=`right
            className={Css.style([
              FC.PageCard.HeaderRow.Styles.itemTopPadding,
              FC.PageCard.HeaderRow.Styles.itemBottomPadding,
            ])}>
            <FC.Base.Button
              variant=Primary
              onClick={e =>
                Foretold__Components__Link.LinkType.onClick(
                  Internal(ChannelInvite(channelId)),
                  e,
                )
              }>
              {"Add Members" |> ste}
            </FC.Base.Button>
          </FC.Base.Div>
        </>,
      ~body=<FC.PageCard.Body> table </FC.PageCard.Body>,
    )
    |> layout;
  },
};