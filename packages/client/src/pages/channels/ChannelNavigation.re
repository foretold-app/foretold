let component = ReasonReact.statelessComponent("Channel");

let make =
    (
      ~channelPage: Routing.ChannelPage.t,
      ~loggedInUser: option(Types.user),
      _children,
    ) => {
  ...component,
  render: _ => {
    let channelId = channelPage.channelId;

    switch (loggedInUser) {
    | Some(loggedInUser) =>
      let successFn = (channel: Types.channel) =>
        <Channel channelPage loggedInUser channel={Some(channel)}>
          {switch (channelPage.subPage) {
           | Measurables(searchParams) =>
             <MeasurableIndex channelId searchParams loggedInUser />
           | Measurable(measurableId) =>
             <ChannelMeasurable
               channelId={Some(channelId)}
               measurableId
               loggedInUser
             />
           | Series(id) => <SeriesShow id channelId loggedInUser />
           | NewMeasurable => <MeasurableNew channelId loggedInUser />
           | Members => <ChannelMembers channelId channel />
           | FeedItems => <FeedItems channelId={Some(channelId)} />
           | Leaderboard(ByMeasurable) =>
             <LeaderboardMeasurables channelId={Some(channelId)} />
           | Leaderboard(ByMember) =>
             <LeaderboardMembers channelId={Some(channelId)} />
           | AddMember => <ChannelAddMember channelId loggedInUser />
           | InviteMember => <ChannelInviteMember channelId loggedInUser />
           | Settings => <ChannelEdit channelId loggedInUser />
           | NewSeries => <SeriesNew channelId loggedInUser />
           }}
        </Channel>;

      let errorFn = _ =>
        <Channel channelPage loggedInUser channel=None>
          <SLayout>
            <div> {"No channel." |> ReasonReact.string} </div>
          </SLayout>
        </Channel>;

      let loadingFn = () =>
        <Channel channelPage loggedInUser channel=None>
          <SLayout> <Spin /> </SLayout>
        </Channel>;

      ChannelGet.component2(~id=channelId, result =>
        result |> HttpResponse.flatten(successFn, errorFn, loadingFn)
      );
    | None => <Home />
    };
  },
};
