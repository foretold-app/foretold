let newMeasurable = channelId =>
  Fc.FC.GroupHeader.actionButton(
    ~onClick=e => LinkType.onClick(Internal(MeasurableNew(channelId)), e),
    [|"New Question" |> ReasonReact.string|],
  )
  |> ReasonReact.element;

let leaveChannel = (channelId: string) =>
  ChannelLeave.Mutation.make((mutation, _) =>
    Fc.FC.GroupHeader.actionButton(
      ~variant=Secondary,
      ~onClick=_ => ChannelLeave.mutate(mutation, channelId),
      [|"Leave Community" |> ReasonReact.string|],
    )
    |> E.React.el
  )
  |> E.React.el;

let joinChannel = channelId =>
  ChannelJoin.Mutation.make((mutation, _) =>
    Fc.FC.GroupHeader.actionButton(
      ~onClick=_ => ChannelJoin.mutate(mutation, channelId),
      [|"Join Community" |> ReasonReact.string|],
    )
    |> E.React.el
  )
  |> E.React.el;