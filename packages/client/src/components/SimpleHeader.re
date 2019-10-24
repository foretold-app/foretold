[@bs.config {jsx: 3}];

let newMeasurable = channelId => {
  <FC__Button
    variant=FC__Button.Primary
    isDisabled=false
    size=FC__Button.(Medium)
    className=FC.GroupHeader.Styles.actionButtonPosition
    onClick={e => LinkType.onClick(Internal(MeasurableNew(channelId)), e)}>
    {"New Question" |> ReasonReact.string}
  </FC__Button>;
};

let leaveChannel = (channelId: string) =>
  <ChannelLeave.Mutation>
    ...{(mutation, _) =>
      <FC__Button
        variant=FC__Button.Secondary
        isDisabled=false
        size=FC__Button.(Medium)
        className=FC.GroupHeader.Styles.actionButtonPosition
        onClick={_ => ChannelLeave.mutate(mutation, channelId)}>
        {"eave Community" |> ReasonReact.string}
      </FC__Button>
    }
  </ChannelLeave.Mutation>;

let joinChannel = channelId =>
  ChannelJoin.Mutation.make((mutation, _) =>
    FC.GroupHeader.actionButton(
      ~onClick=_ => ChannelJoin.mutate(mutation, channelId),
      [|"Join Community" |> ReasonReact.string|],
    )
    |> E.React.el
  )
  |> E.React.el;
