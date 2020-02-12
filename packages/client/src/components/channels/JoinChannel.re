[@react.component]
let make = (~channelId) =>
  <ChannelJoin.Mutation>
    ...{(mutation, _) =>
      <ForetoldComponents.Button
        variant=ForetoldComponents.Button.Primary
        size=ForetoldComponents.Button.(Small)
        className=ForetoldComponents.GroupHeader.Styles.actionButtonPosition
        onClick={_ => ChannelJoin.mutate(mutation, channelId)}>
        {"Join Community" |> Utils.ste}
      </ForetoldComponents.Button>
    }
  </ChannelJoin.Mutation>;