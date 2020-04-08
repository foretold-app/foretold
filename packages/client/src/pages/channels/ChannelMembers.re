// @todo: To make component.
let changeRoleAction = (agentId, channelId, role, text) =>
  <ChannelMembershipRoleUpdate.Mutation>
    ...{(mutation, _) =>
      <Link
        linkType={
          Action(
            _ =>
              ChannelMembershipRoleUpdate.mutate(
                mutation,
                ~agentId,
                ~channelId,
                ~role,
              ),
          )
        }>
        {text |> Utils.ste}
      </Link>
    }
  </ChannelMembershipRoleUpdate.Mutation>;

// @todo: To make component.
let removeFromChannel = (agentId, channelId) =>
  <ChannelMembershipDelete.Mutation>
    ...{(mutation, _) =>
      <Link
        linkType={
          Action(
            _ => ChannelMembershipDelete.mutate(mutation, agentId, channelId),
          )
        }>
        {"Remove" |> Utils.ste}
      </Link>
    }
  </ChannelMembershipDelete.Mutation>;

module Columns = {
  type column = Table.column(Types.channelMembership);

  let agent =
    Table.Column.make(
      ~name="Member" |> Utils.ste,
      ~render=
        (membership: Types.channelMembership) =>
          membership.agent
          |> Rationale.Option.fmap((agent: Types.agent) =>
               <AgentLink agent />
             )
          |> E.O.React.defaultNull,
      (),
    );

  let role =
    Table.Column.make(
      ~name="Role" |> Utils.ste,
      ~render=
        (membership: Types.channelMembership) =>
          <>
            {switch (membership.role) {
             | `ADMIN =>
               <div className="ant-tag ant-tag-blue">
                 {"Admin" |> Utils.ste}
               </div>
             | `VIEWER =>
               <div className="ant-tag ant-tag-green">
                 {"Member" |> Utils.ste}
               </div>
             }}
            {switch (membership.isVerified) {
             | Some(true) =>
               <div className="ant-tag ant-tag-magenta">
                 {"Verified" |> Utils.ste}
               </div>
             | _ => <Null />
             }}
          </>,
      (),
    );

  let roleChange = channelId =>
    Table.Column.make(
      ~name="Change Role" |> Utils.ste,
      ~render=
        (membership: Types.channelMembership) =>
          <div>
            {switch (membership.role, membership.agent) {
             | (`VIEWER, Some(agent)) =>
               E.React2.showIf(
                 Primary.Permissions.can(
                   `CHANNEL_MEMBERSHIP_ROLE_UPDATE,
                   membership.permissions,
                 ),
                 changeRoleAction(
                   agent.id,
                   channelId,
                   `ADMIN,
                   "Change to Admin",
                 ),
               )
             | (`ADMIN, Some(agent)) =>
               E.React2.showIf(
                 Primary.Permissions.can(
                   `CHANNEL_MEMBERSHIP_ROLE_UPDATE,
                   membership.permissions,
                 ),
                 changeRoleAction(
                   agent.id,
                   channelId,
                   `VIEWER,
                   "Change to Member",
                 ),
               )
             | _ => <div />
             }}
          </div>,
      (),
    );

  let removeFromChannel = channelId =>
    Table.Column.make(
      ~name="Remove" |> Utils.ste,
      ~render=
        (membership: Types.channelMembership) =>
          switch (
            membership.agent,
            Primary.Permissions.can(
              `CHANNEL_MEMBERSHIP_DELETE,
              membership.permissions,
            ),
          ) {
          | (Some(agent), true) => removeFromChannel(agent.id, channelId)
          | _ => <Null />
          },
      (),
    );

  let all = (channelId: string, channel: Types.channel) => {
    switch (channel.myRole) {
    | Some(`ADMIN) => [|
        agent,
        role,
        roleChange(channelId),
        removeFromChannel(channelId),
      |]
    | _ => [|agent, role|]
    };
  };
};

let title = () =>
  <ForetoldComponents.Base.Div float=`left>
    <ForetoldComponents.PageCard.HeaderRow.Title>
      {"Community Members" |> Utils.ste}
    </ForetoldComponents.PageCard.HeaderRow.Title>
  </ForetoldComponents.Base.Div>;

let addMembersButtonSection = (channelId: string) =>
  <ForetoldComponents.Base.Div
    float=`right
    className={Css.style([
      ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
      ForetoldComponents.PageCard.HeaderRow.Styles.itemRightPadding,
    ])}>
    <ForetoldComponents.Base.Button
      variant=ForetoldComponents.Base.Button.Primary
      size=ForetoldComponents.Base.Button.MediumShort
      onClick={e =>
        LinkType.onClick(Internal(ChannelAddMember(channelId)), e)
      }>
      {"Add Members" |> Utils.ste}
    </ForetoldComponents.Base.Button>
  </ForetoldComponents.Base.Div>;

// Currently seems broken, so I removed it for now. Add to "SuccesFn" later when fixed.
let inviteMemberButtonSection = (channelId: string) =>
  <ForetoldComponents.Base.Div
    float=`right
    className={Css.style([
      ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
      ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
    ])}>
    <ForetoldComponents.Base.Button
      variant=ForetoldComponents.Base.Button.Secondary
      size=ForetoldComponents.Base.Button.MediumShort
      onClick={e =>
        LinkType.onClick(Internal(ChannelInviteMember(channelId)), e)
      }>
      {"Invite Member With Email" |> Utils.ste}
    </ForetoldComponents.Base.Button>
  </ForetoldComponents.Base.Div>;

let succesFn = (~channelId: string, ~channel: Types.channel, ~memberships) => {
  let head =
    switch (channel.myRole) {
    | Some(`ADMIN) =>
      <div> {title()} {addMembersButtonSection(channelId)} </div>
    | _ => <div> {title()} </div>
    };

  let table =
    <Table columns={Columns.all(channelId, channel)} rows=memberships />;

  <SLayout head>
    <ForetoldComponents.PageCard.Body>
      table
    </ForetoldComponents.PageCard.Body>
  </SLayout>;
};

let errorFn = _ => <NotFoundPage />;

let loadingFn = _ => <SLayout> <Spin /> </SLayout>;

[@react.component]
let make = (~channelId: string, ~channel: Types.channel) => {
  ChannelMembershipsGet.component(~id=channelId, result =>
    result
    |> HttpResponse.flatten(
         memberships => succesFn(~channelId, ~channel, ~memberships),
         errorFn,
         loadingFn,
       )
  );
};