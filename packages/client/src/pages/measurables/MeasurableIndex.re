[@bs.config {jsx: 3}];

[@react.component]
let make = (~channelId: string, ~searchParams: MeasurableQueryIndex.query) => {
  module Reducer =
    PaginationFunctor.Make(MeasurableIndex__Logic.ReducerConfig);
  <Reducer
    callFnParams={channelId, states: searchParams.state}
    subComponent={(reducerParams: Reducer.Types.reducerParams) =>
      ChannelGet.component2(~id=channelId, channelQuery =>
        SeriesCollectionGet.component2(~channelId, seriesQuery =>
          MeasurablesStateStatsGet.component2(~channelId, statsQuery =>
            MeasurableIndex__Components.toLayoutInput(
              reducerParams.send,
              searchParams,
              statsQuery,
              MeasurableIndex__Logic.make({
                reducerParams,
                channelQuery,
                seriesQuery,
              }),
            )
          )
        )
      )
    }
  />;
};
