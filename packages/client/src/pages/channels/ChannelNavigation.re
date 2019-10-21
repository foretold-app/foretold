let component = ReasonReact.statelessComponent("Channel");

let make =
    (
      ~channelPage: Routing.ChannelPage.t,
      ~loggedUser: option(Types.user),
      _children,
    ) => {
  ...component,
  render: _ => {
    let channelId = channelPage.channelId;

    switch (loggedUser) {
    | Some(loggedUser) =>
      let loadChannel = ChannelGet.component2(~id=channelId);

      let successFn = (channel: Types.channel) =>
        <Channel channelPage loggedUser channel={Some(channel)}>
          {switch (channelPage.subPage) {
           | Measurables(searchParams) =>
             <MeasurableIndex channelId searchParams loggedUser />
           | Measurable(measurableId) =>
             <ChannelMeasurable
               channelId={Some(channelId)}
               measurableId
               loggedUser
             />
           | Series(id) => <SeriesShow id channelId loggedUser />
           | NewMeasurable => <MeasurableNew channelId loggedUser />
           | Members => <ChannelMembers channelId channel />
           | FeedItems => <FeedItems channelId={Some(channelId)} />
           | Leaderboard(ByMeasurable) =>
             <LeaderboardMeasurables channelId={Some(channelId)} />
           | Leaderboard(ByMember) =>
             <LeaderboardMembers channelId={Some(channelId)} />
           | AddMember => <ChannelAddMember channelId loggedUser />
           | InviteMember => <ChannelInviteMember channelId loggedUser />
           | Settings => <ChannelEdit channelId loggedUser />
           | NewSeries => <SeriesNew channelId loggedUser />
           }}
        </Channel>;

      let errorFn = _ =>
        <Channel channelPage loggedUser channel=None>
          <SLayout>
            <div> {"No channel." |> ReasonReact.string} </div>
          </SLayout>
        </Channel>;

      let loadingFn = () =>
        <Channel channelPage loggedUser channel=None>
          <SLayout> <Spin /> </SLayout>
        </Channel>;

      loadChannel(result =>
        result |> HttpResponse.flatten(successFn, errorFn, loadingFn)
      );
    | None => <Home />
    };
  },
};