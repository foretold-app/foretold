module Reducer = PaginationFunctor.Make(MeasurableIndex__Logic.ReducerConfig);

[@react.component]
let make = (~channelId: string, ~searchParams: MeasurableQuery.query) => {
  <Reducer
    callFnParams={channelId, states: searchParams.state}
    subComponent={(reducerParams: Reducer.reducerParams) =>
      ChannelGet.component2(~id=channelId, channelQuery =>
        SeriesCollectionGet.component2(~channelId, seriesQuery =>
          MeasurablesStateStatsGet.component2(
            ~channelId,
            statsQuery => {
              let state =
                MeasurableIndex__Logic.getState({
                  reducerParams,
                  channelQuery,
                  seriesQuery,
                });

              MeasurableIndex__Components.toLayoutInput(
                reducerParams.send,
                searchParams,
                statsQuery,
                state,
                channelId,
              );
            },
          )
        )
      )
    }
  />;
};