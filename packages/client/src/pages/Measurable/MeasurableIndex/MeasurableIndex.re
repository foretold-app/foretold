open Foretold__GraphQL;

module Types = Measurable__Index__Logic;
module Components = Measurable__Index__Components;

let load3Queries = (channelId, states, itemsPerPage, fn) =>
  ((a, b, c, d) => (a, b, c, d) |> fn)
  |> E.F.flatten4Callbacks(
       Types.SelectWithPaginationReducer.make(
         ~itemsPerPage,
         ~callFnParams={channelId, states},
         ~subComponent=_,
       ),
       Queries.Channel.component2(~id=channelId),
       Queries.SeriesCollection.component2(~channelId),
       Queries.MeasurablesStateStats.component2(~channelId),
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
  loadData(
    ((selectWithPaginationParams, channel, query, measurablesStateStats)) =>
    Types.MeasurableIndexDataState.make({
      reducerParams: selectWithPaginationParams,
      loggedInUser,
      channel,
      query,
    })
    |> Components.MeasurableIndexDataState.toLayoutInput(
         selectWithPaginationParams.send,
         searchParams,
         measurablesStateStats,
       )
    |> layout
  );
};