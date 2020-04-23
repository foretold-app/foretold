[@react.component]
let make = (~channelPage: Routing.ChannelPage.t) => {
  let channelId = channelPage.channelId;

  let successFn = (channel: Types.channel) => {
    <Channel channelPage channel={Some(channel)}>
      {switch (channelPage.subPage) {
       // Measurables
       | Measurables(searchParams) =>
         <MeasurableIndex channelId searchParams />
       | Measurable(measurableId) => <ChannelMeasurable measurableId />
       | NewMeasurable => <MeasurableNew channelId />

       // Leaderboards
       | Leaderboard(ByMeasurable) =>
         <LeaderboardMeasurables channelId={Some(channelId)} />
       | Leaderboard(ByMember) =>
         <LeaderboardMembers channelId={Some(channelId)} />

       // Members
       | Members => <ChannelMembers channelId channel />
       | AddMember => <ChannelAddMember channelId />
       | InviteMember => <ChannelInviteMember channelId />

       // Series
       | NewSeries => <SeriesNew channelId />
       | Series(seriesId) => <SeriesPage seriesId channelId />
       | SeriesEdit(seriesId) => <SeriesEdit seriesId />

       // Notebooks
       | Notebook(notebookId) => <NotebookPage notebookId />
       | NotebookDetails(notebookId) => <NotebookPage notebookId />
       | Notebooks => <Notebooks channelId />
       | AddNotebook => <NotebookCreate channelId />

       // Other
       | Settings => <ChannelEdit channelId />
       | FeedItems => <FeedItemsIndex channelId={Some(channelId)} />
       | Unknown => <NotFoundPage />
       | _ => <NotFoundPage />
       }}
    </Channel>;
  };

  let errorFn = _ =>
    <Channel channelPage channel=None> <NotFoundPage /> </Channel>;

  let loadingFn = () =>
    <Channel channelPage channel=None>
      <SLayout> <Spin /> </SLayout>
    </Channel>;

  ChannelGet.component2(~id=channelId, result =>
    result |> HttpResponse.flatten(successFn, errorFn, loadingFn)
  );
};