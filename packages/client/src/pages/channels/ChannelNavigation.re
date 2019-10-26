let component = ReasonReact.statelessComponent("Channel");

let make = (~channelPage: Routing.ChannelPage.t, _children) => {
  ...component,
  render: _ => {
    let channelId = channelPage.channelId;

    let loadChannel = ChannelGet.component2(~id=channelId);

    let successFn = (channel: Types.channel) =>
      <Channel channelPage channel={Some(channel)}>
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
         | Dashboard => <Dashboard channelId />
         }}
      </Channel>;

    let errorFn = _ =>
      <Channel channelPage channel=None>
        <SLayout> <div> {"No channel." |> ReasonReact.string} </div> </SLayout>
      </Channel>;

    let loadingFn = () =>
      <Channel channelPage channel=None>
        <SLayout> <Spin /> </SLayout>
      </Channel>;

    loadChannel(result =>
      result |> HttpResponse.flatten(successFn, errorFn, loadingFn)
    );
  },
};