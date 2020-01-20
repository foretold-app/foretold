module LeaveChannel = {
  [@react.component]
  let make = (~channelId) =>
    <ChannelLeave.Mutation>
      ...{(mutation, result: ChannelLeave.Mutation.renderPropObj) =>
        <Antd.Popconfirm
          title={Lang.areYouSure |> Utils.ste}
          onConfirm={_ => ChannelLeave.mutate(mutation, channelId)}>
          <FC__Button
            variant=FC__Button.Secondary
            isDisabled=false
            size=FC__Button.(Medium)
            className=FC.GroupHeader.Styles.actionButtonPosition>
            {"Leave Community" |> Utils.ste}
          </FC__Button>
        </Antd.Popconfirm>
      }
    </ChannelLeave.Mutation>;
};

module JoinChannel = {
  [@react.component]
  let make = (~channelId) =>
    <ChannelJoin.Mutation>
      ...{(mutation, _) =>
        <FC__Button
          variant=FC__Button.Primary
          isDisabled=false
          size=FC__Button.(Small)
          className=FC.GroupHeader.Styles.actionButtonPosition
          onClick={_ => ChannelJoin.mutate(mutation, channelId)}>
          {"Join Community" |> Utils.ste}
        </FC__Button>
      }
    </ChannelJoin.Mutation>;
};