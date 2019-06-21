let load3Queries = (channelId, states, itemsPerPage, fn) =>
  (
    (reducer, channel, series, stats) =>
      (reducer, channel, series, stats) |> fn
  )
  |> E.F.flatten4Callbacks(
       Measurable__Index__Logic.Reducer.make(
         ~itemsPerPage,
         ~callFnParams={channelId, states},
         ~subComponent=_,
       ),
       Foretold__GraphQL.Queries.Channel.component2(~id=channelId),
       Foretold__GraphQL.Queries.SeriesCollection.component2(~channelId),
       Foretold__GraphQL.Queries.MeasurablesStateStats.component2(~channelId),
     );

let make =
    (
      ~channelId: string,
      ~searchParams: Context.QueryParams.MeasurableIndex.query,
      ~loggedInUser: Context.Primary.User.t,
      ~itemsPerPage: int=20,
      ~layout,
    ) => {
  let loadData =
    load3Queries(
      channelId,
      [|
        searchParams.state |> E.O.toExn("This should not have been possible."),
      |],
      itemsPerPage,
    );

  loadData(((reducerParams, channel, query, measurablesStateStats)) =>
    Measurable__Index__Logic.MeasurableIndexDataState.make({
      reducerParams,
      loggedInUser,
      channel,
      query,
    })
    |> Measurable__Index__Components.MeasurableIndexDataState.toLayoutInput(
         reducerParams.send,
         searchParams,
         measurablesStateStats,
       )
    |> layout
  );
};