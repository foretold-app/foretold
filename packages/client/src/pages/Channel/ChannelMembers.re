open Foretold__GraphQL;

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
      {text |> ReasonReact.string}
    </Foretold__Components__Link>
  )
  |> ReasonReact.element;

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
      {"Remove" |> ReasonReact.string}
    </Foretold__Components__Link>
  )
  |> ReasonReact.element;

module Columns = {
  type column = Table.column(Context.Primary.Types.channelMembership);

  let canX =
      (
        permission: Context.Primary.permission,
        record: Context.Primary.Types.channelMembership,
      ) =>
    record.permissions
    |> Rationale.Option.fmap((permissions: Context.Primary.Permissions.t) =>
         Context.Primary.Permissions.canX(permission, permissions)
       )
    |> Rationale.Option.default(false);

  let agentColumn: column = {
    name: "Agent" |> ReasonReact.string,
    render: membership =>
      membership.agent
      |> Rationale.Option.fmap((r: Context.Primary.Types.agent) =>
           <Foretold__Components__Link
             linkType={
               Internal(Agent({agentId: r.id, subPage: AgentMeasurements}))
             }>
             {r.name
              |> Rationale.Option.default("Anonymous")
              |> ReasonReact.string}
           </Foretold__Components__Link>
         )
      |> E.O.React.defaultNull,
    flex: 1,
  };

  let roleColumn: column = {
    name: "Role" |> ReasonReact.string,
    render: membership =>
      switch (membership.role) {
      | `ADMIN =>
        <div className="ant-tag ant-tag-blue">
          {"Admin" |> ReasonReact.string}
        </div>
      | `VIEWER =>
        <div className="ant-tag ant-tag-green">
          {"Viewer" |> ReasonReact.string}
        </div>
      },
    flex: 1,
  };

  let roleChangeColumn: string => column =
    channelId => {
      name: "Change Role" |> ReasonReact.string,
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
      name: "Remove" |> ReasonReact.string,
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

  let all = (channelId: string, channel: Context.Primary.Types.channel) => {
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

let title = () =>
  <FC.Base.Div float=`left>
    <FC.PageCard.HeaderRow.Title>
      {"Community Members" |> ReasonReact.string}
    </FC.PageCard.HeaderRow.Title>
  </FC.Base.Div>;

let addMembersButtonSection = (channelId: string) =>
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
      {"Add Members" |> ReasonReact.string}
    </FC.Base.Button>
  </FC.Base.Div>;

let succesFn =
    (
      ~channelId: string,
      ~layout,
      ~channel: Context.Primary.Types.channel,
      ~memberships,
    ) => {
  let head =
    switch (channel.myRole) {
    | Some(`ADMIN) =>
      <div> {title()} {addMembersButtonSection(channelId)} </div>
    | _ => <div> {title()} </div>
    };

  let table =
    Table.fromColumns(Columns.all(channelId, channel), memberships);

  SLayout.LayoutConfig.make(
    ~head,
    ~body=<FC.PageCard.Body> table </FC.PageCard.Body>,
  )
  |> layout;
};

let errorFn = (layout, _) => {
  SLayout.LayoutConfig.make(
    ~head=<div />,
    ~body=<div> {"No channel." |> ReasonReact.string} </div>,
  )
  |> layout;
};

let loadingFn = (layout, _) => {
  SLayout.LayoutConfig.make(~head=<div />, ~body=<SLayout.Spin />) |> layout;
};

let make =
    (
      ~channelId: string,
      ~layout=SLayout.FullPage.makeWithEl,
      ~channel: Context.Primary.Channel.t,
      _children,
    ) => {
  ...component,
  render: _ => {
    Queries.ChannelMemberships.component(~id=channelId, result =>
      result
      |> E.HttpResponse.flatten(
           memberships =>
             succesFn(~channelId, ~layout, ~channel, ~memberships),
           errorFn(layout),
           loadingFn(layout),
         )
    );
  },
};