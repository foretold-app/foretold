[@bs.config {jsx: 3}];

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
        {text |> ReasonReact.string}
      </Link>
    }
  </ChannelMembershipRoleUpdate.Mutation>;

let removeFromChannel = (agentId, channelId) =>
  ChannelMembershipDelete.Mutation.make((mutation, _) =>
    <Link
      linkType={
        Action(
          _ => ChannelMembershipDelete.mutate(mutation, agentId, channelId),
        )
      }>
      {"Remove" |> ReasonReact.string}
    </Link>
  )
  |> ReasonReact.element;

module Columns = {
  type column = Table.column(Types.channelMembership);

  let agentColumn =
    Table.Column.make(
      ~name="Member" |> ReasonReact.string,
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
      ~name="Role" |> ReasonReact.string,
      ~render=
        (membership: Types.channelMembership) =>
          switch (membership.role) {
          | `ADMIN =>
            <div className="ant-tag ant-tag-blue">
              {"Admin" |> ReasonReact.string}
            </div>
          | `VIEWER =>
            <div className="ant-tag ant-tag-green">
              {"Member" |> ReasonReact.string}
            </div>
          },
      (),
    );

  let roleChangeColumn: string => column =
    channelId =>
      Table.Column.make(
        ~name="Change Role" |> ReasonReact.string,
        ~render=
          (membership: Types.channelMembership) =>
            <div>
              {switch (membership.role, membership.agent) {
               | (`VIEWER, Some(agent)) =>
                 E.React.showIf(
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
                 E.React.showIf(
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
        ~name="Remove" |> ReasonReact.string,
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
            | _ => ReasonReact.null
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
      {"Community Members" |> ReasonReact.string}
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
      variant=Primary
      size=MediumShort
      onClick={e =>
        LinkType.onClick(Internal(ChannelAddMember(channelId)), e)
      }>
      {"Add Members" |> ReasonReact.string}
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
      variant=Secondary
      size=MediumShort
      onClick={e =>
        LinkType.onClick(Internal(ChannelInviteMember(channelId)), e)
      }>
      {"Invite Member With Email" |> ReasonReact.string}
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
    Table.fromColumns(Columns.all(channelId, channel), memberships, ());

  <SLayout head> <FC.PageCard.Body> table </FC.PageCard.Body> </SLayout>;
};

let errorFn = _ =>
  <SLayout> <div> {"No channel." |> ReasonReact.string} </div> </SLayout>;

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
