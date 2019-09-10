let make =
    (
      ~channelId: string,
      ~searchParams: MeasurableQueryIndex.query,
      ~loggedInUser: Types.user,
    ) => {
  let loadData = fn =>
    (
      fnlast =>
        (
          MeasurableIndex__Logic.Reducer.make(
            ~callFnParams={channelId, states: searchParams.state},
            ~subComponent=_,
          )
        )(
          response1 =>
          ChannelGet.component2(~id=channelId, response2 =>
            SeriesCollectionGet.component2(~channelId, response3 =>
              MeasurablesStateStatsGet.component2(~channelId, response4 =>
                fnlast(response1, response2, response3, response4)
              )
            )
          )
        )
    )(
      (reducer, channel, series, stats) =>
      (reducer, channel, series, stats)
      |> (
        (
          (
            reducerParams,
            channelQuery,
            seriesQuery,
            measurablesStateStatsQuery,
          ),
        ) =>
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
    );

  loadData();
};