/* TODO: Allow for cases where user is not logged in */
let makeWithPage =
    (channelPage: Context.Routing.ChannelPage.t, loggedInUser)
    : ReasonReact.reactElement => {
  let channelId = channelPage.channelId;

  switch (loggedInUser) {
  | Some(loggedInUser) =>
    let layout = Channel_Layout_C.makeWithEl(channelPage, loggedInUser);

    let toEl = fn => fn |> E.React.makeToEl(~key=channelId);

    switch (channelPage.subPage) {
    | Measurables(searchParams) =>
      MeasurableIndex.make(
        ~channelId,
        ~searchParams,
        ~loggedInUser,
        ~itemsPerPage=20,
        ~layout,
      )
      |> toEl
    | Series(id) =>
      SeriesShow.make(~id, ~channelId, ~loggedInUser, ~layout) |> toEl
    | NewMeasurable => MeasurableNew.make(~channelId, ~layout) |> toEl
    | Members => ChannelMembers.make(~channelId, ~layout) |> toEl
    | InviteNewMember =>
      ChannelInvite.make(~channelId, ~loggedInUser, ~layout) |> toEl
    | Settings => ChannelEdit.make(~channelId, ~layout) |> toEl
    | NewSeries => SeriesNew.make(~channelId, ~loggedInUser, ~layout) |> toEl
    };

  | None => <Home />
  };
};