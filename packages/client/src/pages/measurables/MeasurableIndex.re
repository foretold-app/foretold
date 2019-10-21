let component = ReasonReact.statelessComponent("MeasurableIndex");
let make =
    (
      ~channelId: string,
      ~searchParams: MeasurableQueryIndex.query,
      ~loggedUser: Types.user,
      _children,
    ) => {
  ...component,
  render: _ => {
    module Reducer =
      PaginationFunctor.Make(MeasurableIndex__Logic.ReducerConfig);
    <Reducer
      callFnParams={channelId, states: searchParams.state}
      subComponent={reducerParams =>
        ChannelGet.component2(~id=channelId, channelQuery =>
          SeriesCollectionGet.component2(~channelId, seriesQuery =>
            MeasurablesStateStatsGet.component2(~channelId, statsQuery =>
              MeasurableIndex__Components.toLayoutInput(
                reducerParams.send,
                searchParams,
                statsQuery,
                MeasurableIndex__Logic.make({
                  reducerParams,
                  loggedUser,
                  channelQuery,
                  seriesQuery,
                }),
              )
            )
          )
        )
      }
    />;
  },
};