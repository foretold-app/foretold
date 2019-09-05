/* TODO: Allow for cases where user is not logged in */

let makeWithPage =
    (channelPage: Routing.ChannelPage.t, loggedInUser: option(Types.user))
    : ReasonReact.reactElement => {
  let channelId = channelPage.channelId;

  switch (loggedInUser) {
  | Some(loggedInUser) =>
    let loadChannel = ChannelGet.component2(~id=channelId);

    let successFn = (channel: Types.channel) =>
      <Channel_Layout_C channelPage loggedInUser channel={Some(channel)}>
        {switch (channelPage.subPage) {
         | Measurables(searchParams) =>
           <MeasurableIndex channelId searchParams loggedInUser />
         | Measurable(measurableId) =>
           <ChannelMeasurable measurableId loggedInUser />
         | Series(id) => <SeriesShow id channelId loggedInUser />
         | NewMeasurable => <MeasurableNew channelId loggedInUser />
         | Members => <ChannelMembers channelId channel />
         | FeedItems => <FeedItems channelId={Some(channelId)} />
         | Leaderboard(ByMeasurement) =>
           <LeaderboardMeasurements
             channelId={Some(channelId)}
             subTab=ByMeasurement
           />
         | Leaderboard(ByMeasurable) =>
           <LeaderboardMeasurables
             channelId={Some(channelId)}
             subTab=ByMeasurable
           />
         | Leaderboard(ByMember) =>
           <LeaderboardMembers channelId={Some(channelId)} subTab=ByMember />
         | AddMember => <ChannelAddMember channelId loggedInUser />
         | InviteMember => <ChannelInviteMember channelId loggedInUser />
         | Settings => <ChannelEdit channelId loggedInUser />
         | NewSeries => <SeriesNew channelId loggedInUser />
         }}
      </Channel_Layout_C>;

    let errorFn = _ => {
      SLayout.LayoutConfig.make(
        ~head=<div />,
        ~body=<div> {"No channel." |> ReasonReact.string} </div>,
      )
      |> SLayout.FullPage.makeWithEl;
    };

    let loadingFn = () => {
      SLayout.LayoutConfig.make(~head=<div />, ~body=<SLayout.Spin />)
      |> SLayout.FullPage.makeWithEl;
    };

    loadChannel(result =>
      result |> HttpResponse.flatten(successFn, errorFn, loadingFn)
    );
  | None => <Home />
  };
};