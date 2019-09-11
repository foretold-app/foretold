let make =
    (
      ~channelId: string,
      ~searchParams: MeasurableQueryIndex.query,
      ~loggedInUser: Types.user,
    ) => {
  MeasurableIndex__Logic.Reducer.make(
    ~callFnParams={channelId, states: searchParams.state},
    ~subComponent=reducerParams =>
    ChannelGet.component2(~id=channelId, channelQuery =>
      SeriesCollectionGet.component2(~channelId, seriesQuery =>
        MeasurablesStateStatsGet.component2(
          ~channelId, measurablesStateStatsQuery =>
          MeasurableIndex__Components.MeasurableIndexDataState.toLayoutInput(
            reducerParams.send,
            searchParams,
            measurablesStateStatsQuery,
            MeasurableIndex__Logic.MeasurableIndexDataState.make({
              reducerParams,
              loggedInUser,
              channelQuery,
              seriesQuery,
            }),
          )
        )
      )
    )
  );
};