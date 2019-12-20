// @todo: To make a component.
let newMeasurable = channelId => {
  <FC__Button
    variant=FC__Button.Primary
    isDisabled=false
    size=FC__Button.(Medium)
    className=FC.GroupHeader.Styles.actionButtonPosition
    onClick={e => LinkType.onClick(Internal(MeasurableNew(channelId)), e)}>
    {"New Question" |> Utils.ste}
  </FC__Button>;
};

// @todo: To make a component.
let leaveChannel = (channelId: string) =>
  <ChannelLeave.Mutation>
    ...{(mutation, _) =>
      <FC__Button
        variant=FC__Button.Secondary
        isDisabled=false
        size=FC__Button.(Medium)
        className=FC.GroupHeader.Styles.actionButtonPosition
        onClick={_ => ChannelLeave.mutate(mutation, channelId)}>
        {"Leave Community" |> Utils.ste}
      </FC__Button>
    }
  </ChannelLeave.Mutation>;

// @todo: To make a component.
let joinChannel = channelId =>
  <ChannelJoin.Mutation>
    ...{(mutation, _) =>
      <FC__Button
        variant=FC__Button.Primary
        isDisabled=false
        size=FC__Button.(Medium)
        className=FC.GroupHeader.Styles.actionButtonPosition
        onClick={_ => ChannelJoin.mutate(mutation, channelId)}>
        {"Join Community" |> Utils.ste}
      </FC__Button>
    }
  </ChannelJoin.Mutation>;