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
            size=ForetoldComponents.Button.Medium
            className=ForetoldComponents.GroupHeader.Styles.actionButtonPosition>
            {"Leave Community" |> Utils.ste}
          </ForetoldComponents.Button>
          <div className=ForetoldComponents.BaseStyles.clear />
        </div>
      </Antd.Popconfirm>
    }
  </ChannelLeave.Mutation>;