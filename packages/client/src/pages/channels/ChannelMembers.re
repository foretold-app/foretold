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

  let agentColumn =
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

  let roleColumn =
    Table.Column.make(
      ~name="Role" |> Utils.ste,
      ~render=
        (membership: Types.channelMembership) =>
          switch (membership.role) {
          | `ADMIN =>
            <div className="ant-tag ant-tag-blue">
              {"Admin" |> Utils.ste}
            </div>
          | `VIEWER =>
            <div className="ant-tag ant-tag-green">
              {"Member" |> Utils.ste}
            </div>
          },
      (),
    );

  let roleChangeColumn: string => column =
    channelId =>
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

  let removeFromChannelColumn: string => column =
    channelId =>
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
      {"Community Members" |> Utils.ste}
    </FC.PageCard.HeaderRow.Title>
  </FC.Base.Div>;

let addMembersButtonSection = (channelId: string) =>
  <FC.Base.Div
    float=`right
    className={Css.style([
      FC.PageCard.HeaderRow.Styles.itemTopPadding,
      FC.PageCard.HeaderRow.Styles.itemRightPadding,
    ])}>
    <FC.Base.Button
      variant=FC.Base.Button.Primary
      size=FC.Base.Button.MediumShort
      onClick={e =>
        LinkType.onClick(Internal(ChannelAddMember(channelId)), e)
      }>
      {"Add Members" |> Utils.ste}
    </FC.Base.Button>
  </FC.Base.Div>;

let inviteMemberButtonSection = (channelId: string) =>
  <FC.Base.Div
    float=`right
    className={Css.style([
      FC.PageCard.HeaderRow.Styles.itemTopPadding,
      FC.PageCard.HeaderRow.Styles.itemBottomPadding,
    ])}>
    <FC.Base.Button
      variant=FC.Base.Button.Secondary
      size=FC.Base.Button.MediumShort
      onClick={e =>
        LinkType.onClick(Internal(ChannelInviteMember(channelId)), e)
      }>
      {"Invite Member With Email" |> Utils.ste}
    </FC.Base.Button>
  </FC.Base.Div>;

let succesFn = (~channelId: string, ~channel: Types.channel, ~memberships) => {
  let head =
    switch (channel.myRole) {
    | Some(`ADMIN) =>
      <div>
        {title()}
        {inviteMemberButtonSection(channelId)}
        {addMembersButtonSection(channelId)}
      </div>
    | _ => <div> {title()} </div>
    };

  let table =
    <Table columns={Columns.all(channelId, channel)} rows=memberships />;

  <SLayout head> <FC.PageCard.Body> table </FC.PageCard.Body> </SLayout>;
};

let errorFn = _ =>
  <SLayout> <div> {"No channel." |> Utils.ste} </div> </SLayout>;

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