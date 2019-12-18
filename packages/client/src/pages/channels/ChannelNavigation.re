[@react.component]
let make = (~channelPage: Routing.ChannelPage.t) => {
  let channelId = channelPage.channelId;

  let loadChannel = ChannelGet.component2(~id=channelId);

  let successFn = (channel: Types.channel) =>
    <Channel channelPage channel={Some(channel)}>
      {switch (channelPage.subPage) {
       // Measurables
       | Measurables(searchParams) =>
         <MeasurableIndex channelId searchParams />
       | Measurable(measurableId) =>
         <ChannelMeasurable channelId={Some(channelId)} measurableId />
       | NewMeasurable => <MeasurableNew channelId />
       | FeedItems => <FeedItems channelId={Some(channelId)} />

       // Leaderboards
       | Leaderboard(ByMeasurable) =>
         <LeaderboardMeasurables channelId={Some(channelId)} />
       | Leaderboard(ByMember) =>
         <LeaderboardMembers channelId={Some(channelId)} />

       // Members
       | Members => <ChannelMembers channelId channel />
       | AddMember => <ChannelAddMember channelId />
       | InviteMember => <ChannelInviteMember channelId />

       | Settings => <ChannelEdit channelId />

       // Series
       | NewSeries => <SeriesNew channelId />
       | Series(id) => <SeriesPage id channelId />

       // Notebooks
       | Notebook(id) => <NotebookPage channelId notebookId=id />
       | NotebookDetails(id) => <NotebookPage channelId notebookId=id />
       | Notebooks => <Notebooks channelId />
       | AddNotebook => <NotebookCreate channelId />

       | Unknown => "Tab is not found" |> Utils.ste
       }}
    </Channel>;

  let errorFn = _ =>
    <Channel channelPage channel=None>
      <SLayout> <div> {"No channel." |> Utils.ste} </div> </SLayout>
    </Channel>;

  let loadingFn = () =>
    <Channel channelPage channel=None>
      <SLayout> <Spin /> </SLayout>
    </Channel>;

  loadChannel(result =>
    result |> HttpResponse.flatten(successFn, errorFn, loadingFn)
  );
};