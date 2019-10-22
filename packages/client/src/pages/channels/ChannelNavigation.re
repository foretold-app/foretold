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

    let loadChannel = ChannelGet.component2(~id=channelId);

    let successFn = (channel: Types.channel) =>
      <Channel channelPage loggedUser channel={Some(channel)}>
        {switch (channelPage.subPage) {
         | Measurables(searchParams) =>
           <MeasurableIndex channelId searchParams />
         | Measurable(measurableId) =>
           <ChannelMeasurable channelId={Some(channelId)} measurableId />
         | Series(id) => <SeriesShow id channelId />
         | NewMeasurable => <MeasurableNew channelId />
         | Members => <ChannelMembers channelId channel />
         | FeedItems => <FeedItems channelId={Some(channelId)} />
         | Leaderboard(ByMeasurable) =>
           <LeaderboardMeasurables channelId={Some(channelId)} />
         | Leaderboard(ByMember) =>
           <LeaderboardMembers channelId={Some(channelId)} />
         | AddMember => <ChannelAddMember channelId />
         | InviteMember => <ChannelInviteMember channelId />
         | Settings => <ChannelEdit channelId />
         | NewSeries => <SeriesNew channelId />
         }}
      </Channel>;

    let errorFn = _ =>
      <Channel channelPage loggedUser channel=None>
        <SLayout> <div> {"No channel." |> ReasonReact.string} </div> </SLayout>
      </Channel>;

    let loadingFn = () =>
      <Channel channelPage loggedUser channel=None>
        <SLayout> <Spin /> </SLayout>
      </Channel>;

    loadChannel(result =>
      result |> HttpResponse.flatten(successFn, errorFn, loadingFn)
    );
  },
};
