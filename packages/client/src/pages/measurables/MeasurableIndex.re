let load3Queries = (channelId, states, itemsPerPage, fn) =>
  (
    (reducer, channel, series, stats) =>
      (reducer, channel, series, stats) |> fn
  )
  |> E.F.flatten4Callbacks(
       MeasurableIndex__Logic.Reducer.make(
         ~itemsPerPage,
         ~callFnParams={channelId, states},
         ~subComponent=_,
       ),
       ChannelGet.component2(~id=channelId),
       SeriesCollectionGet.component2(~channelId),
       MeasurablesStateStatsGet.component2(~channelId),
     );

let make =
    (
      ~channelId: string,
      ~searchParams: MeasurableQueryIndex.query,
      ~loggedInUser: Types.user,
      ~itemsPerPage: int=20,
    ) => {
  let loadData = load3Queries(channelId, searchParams.state, itemsPerPage);

  loadData(
    ((reducerParams, channelQuery, seriesQuery, measurablesStateStatsQuery)) => {
    let state =
      MeasurableIndex__Logic.MeasurableIndexDataState.make({
        reducerParams,
        loggedInUser,
        channelQuery,
        seriesQuery,
      });

    MeasurableIndex__Components.MeasurableIndexDataState.toLayoutInput(
      reducerParams.send,
      searchParams,
      measurablesStateStatsQuery,
      state,
    );
  });
};