let component = ReasonReact.statelessComponent("ChannelMembers");

let changeRoleAction = (agentId, channelId, role, text) =>
  ChannelMembershipRoleUpdate.Mutation.make((mutation, _) =>
    <Link.Jsx2
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
    </Link.Jsx2>
  )
  |> ReasonReact.element;

let removeFromChannel = (agentId, channelId) =>
  ChannelMembershipDelete.Mutation.make((mutation, _) =>
    <Link.Jsx2
      linkType={
        Action(
          _ => ChannelMembershipDelete.mutate(mutation, agentId, channelId),
        )
      }>
      {"Remove" |> ReasonReact.string}
    </Link.Jsx2>
  )
  |> ReasonReact.element;

module Columns = {
  type column = Table.column(Types.channelMembership);

  let agentColumn: column =
    Table.Column.make(
      ~name="Member" |> ReasonReact.string,
      ~render=
        (membership: Types.channelMembership) =>
          membership.agent
          |> Rationale.Option.fmap((r: Types.agent) =>
               <Link.Jsx2
                 linkType={
                   Internal(
                     Agent({agentId: r.id, subPage: AgentMeasurements}),
                   )
                 }>
                 {r.name |> E.O.default("Anonymous") |> ReasonReact.string}
               </Link.Jsx2>
             )
          |> E.O.React.defaultNull,
      (),
    );

  let roleColumn: column =
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
  <Fc.FC.Base.Div float=`left>
    <Fc.FC.PageCard.HeaderRow.Title>
      {"Community Members" |> ReasonReact.string}
    </Fc.FC.PageCard.HeaderRow.Title>
  </Fc.FC.Base.Div>;

let addMembersButtonSection = (channelId: string) =>
  <Fc.FC.Base.Div
    float=`right
    className={Css.style([
      Fc.FC.PageCard.HeaderRow.Styles.itemTopPadding,
      Fc.FC.PageCard.HeaderRow.Styles.itemBottomPadding,
    ])}>
    <Fc.FC.Base.Button
      variant=Primary
      onClick={e =>
        LinkType.onClick(Internal(ChannelAddMember(channelId)), e)
      }>
      {"Add Members" |> ReasonReact.string}
    </Fc.FC.Base.Button>
  </Fc.FC.Base.Div>;

let inviteMemberButtonSection = (channelId: string) =>
  true
    ? ReasonReact.null
    : <Fc.FC.Base.Div
        float=`right
        className={Css.style([
          Fc.FC.PageCard.HeaderRow.Styles.itemTopPadding,
          Fc.FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          Fc.FC.PageCard.HeaderRow.Styles.itemRightPadding,
        ])}>
        <Fc.FC.Base.Button
          variant=Primary
          onClick={e =>
            LinkType.onClick(Internal(ChannelInviteMember(channelId)), e)
          }>
          {"Invite Member" |> ReasonReact.string}
        </Fc.FC.Base.Button>
      </Fc.FC.Base.Div>;

let succesFn = (~channelId: string, ~channel: Types.channel, ~memberships) => {
  let head =
    switch (channel.myRole) {
    | Some(`ADMIN) =>
      <div>
        {title()}
        {addMembersButtonSection(channelId)}
        {inviteMemberButtonSection(channelId)}
      </div>
    | _ => <div> {title()} </div>
    };

  let table =
    Table.fromColumns(Columns.all(channelId, channel), memberships, ());

  SLayout.LayoutConfig.make(
    ~head,
    ~body=<Fc.FC.PageCard.Body> table </Fc.FC.PageCard.Body>,
  )
  |> SLayout.FullPage.makeWithEl;
};

let errorFn = _ =>
  SLayout.LayoutConfig.make(
    ~head=<div />,
    ~body=<div> {"No channel." |> ReasonReact.string} </div>,
  )
  |> SLayout.FullPage.makeWithEl;

let loadingFn = _ =>
  SLayout.LayoutConfig.make(~head=<div />, ~body=<SLayout.Spin />)
  |> SLayout.FullPage.makeWithEl;

let make = (~channelId: string, ~channel: Types.channel, _children) => {
  ...component,
  render: _ => {
    ChannelMembershipsGet.component(~id=channelId, result =>
      result
      |> HttpResponse.flatten(
           memberships => succesFn(~channelId, ~channel, ~memberships),
           errorFn,
           loadingFn,
         )
    );
  },
};