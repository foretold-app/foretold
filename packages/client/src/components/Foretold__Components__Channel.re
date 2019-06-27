module SimpleHeader = {
  let newMeasurable = channelId =>
    FC.GroupHeader.actionButton(
      ~onClick=
        e =>
          Foretold__Components__Link.LinkType.onClick(
            Internal(MeasurableNew(channelId)),
            e,
          ),
      [|"New Question" |> ReasonReact.string|],
    )
    |> ReasonReact.element;

  let leaveChannel = (channelId: string) =>
    Foretold__GraphQL.Mutations.ChannelLeave.Mutation.make((mutation, _) =>
      FC.GroupHeader.actionButton(
        ~variant=Secondary,
        ~onClick=
          _ =>
            Foretold__GraphQL.Mutations.ChannelLeave.mutate(
              mutation,
              channelId,
            ),
        [|"Leave Community" |> ReasonReact.string|],
      )
      |> E.React.el
    )
    |> E.React.el;

  let joinChannel = channelId =>
    Foretold__GraphQL.Mutations.ChannelJoin.Mutation.make((mutation, _) =>
      FC.GroupHeader.actionButton(
        ~onClick=
          _ =>
            Foretold__GraphQL.Mutations.ChannelJoin.mutate(
              mutation,
              channelId,
            ),
        [|"Join Community" |> ReasonReact.string|],
      )
      |> E.React.el
    )
    |> E.React.el;
};