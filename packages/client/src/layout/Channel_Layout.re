let makeWithPage =
    (channelPage: Context.Routing.Route.channelPage, loggedInUser)
    : ReasonReact.reactElement => {
  let channelId = channelPage.channelId;

  let layout = fn =>
    Channel_Layout_C.makeWithEl(channelPage, loggedInUser)
    |> fn
    |> E.React.makeToEl(~key=channelId);

  switch (channelPage.subPage) {
  | Measurables(searchParams) =>
    MeasurableIndex.make(
      ~channelId,
      ~searchParams,
      ~loggedInUser,
      ~itemsPerPage=20,
      ~layout=_,
    )
    |> layout
  | Series(id) =>
    SeriesShow.make(~id, ~channelId, ~loggedInUser, ~layout=_) |> layout
  | NewMeasurable => MeasurableNew.make(~channelId, ~layout=_) |> layout
  | Members => ChannelMembers.make(~channelId, ~layout=_) |> layout
  | InviteNewMember =>
    ChannelInvite.make(~channelId, ~loggedInUser, ~layout=_) |> layout
  | Settings => ChannelEdit.make(~channelId, ~layout=_) |> layout
  | NewSeries =>
    SeriesNew.make(~channelId, ~loggedInUser, ~layout=_) |> layout
  | _ => E.React.null
  };
};