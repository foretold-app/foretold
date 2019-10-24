[@bs.config {jsx: 3}];

let newMeasurable = channelId => {
  <FC__Button
    variant
    isDisabled=false
    size=FC__Button.(Medium)
    className=Css.(merge([FC__Button.Styles.actionButtonPosition]))
    onClick={e => LinkType.onClick(Internal(MeasurableNew(channelId)), e)}>
    {"New Question" |> ReasonReact.string}
  </FC__Button>;
};

let leaveChannel = (channelId: string) =>
  ChannelLeave.Mutation.make((mutation, _) =>
    FC.GroupHeader.actionButton(
      ~variant=Secondary,
      ~onClick=_ => ChannelLeave.mutate(mutation, channelId),
      [|"Leave Community" |> ReasonReact.string|],
    )
    |> E.React.el
  )
  |> E.React.el;

let joinChannel = channelId =>
  ChannelJoin.Mutation.make((mutation, _) =>
    FC.GroupHeader.actionButton(
      ~onClick=_ => ChannelJoin.mutate(mutation, channelId),
      [|"Join Community" |> ReasonReact.string|],
    )
    |> E.React.el
  )
  |> E.React.el;
