module LeaveChannel = {
  [@react.component]
  let make = (~channelId) =>
    <ChannelLeave.Mutation>
      ...{(mutation, _) =>
        <Antd.Popconfirm
          title={Lang.areYouSure |> Utils.ste}
          onConfirm={_ => ChannelLeave.mutate(mutation, channelId)}>
          <div>
            <ForetoldComponents.Button
              variant=ForetoldComponents.Button.Secondary
              isDisabled=false
              size=ForetoldComponents.Button.(Medium)
              className=ForetoldComponents.GroupHeader.Styles.actionButtonPosition>
              {"Leave Community" |> Utils.ste}
            </ForetoldComponents.Button>
            <div className=ForetoldComponents.BaseStyles.clear />
          </div>
        </Antd.Popconfirm>
      }
    </ChannelLeave.Mutation>;
};

module JoinChannel = {
  [@react.component]
  let make = (~channelId) =>
    <ChannelJoin.Mutation>
      ...{(mutation, _) =>
        <ForetoldComponents.Button
          variant=ForetoldComponents.Button.Primary
          isDisabled=false
          size=ForetoldComponents.Button.(Small)
          className=ForetoldComponents.GroupHeader.Styles.actionButtonPosition
          onClick={_ => ChannelJoin.mutate(mutation, channelId)}>
          {"Join Community" |> Utils.ste}
        </ForetoldComponents.Button>
      }
    </ChannelJoin.Mutation>;
};