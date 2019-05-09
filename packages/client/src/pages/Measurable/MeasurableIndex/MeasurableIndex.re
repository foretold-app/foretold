open Foretold__GraphQL;

module Types = Measurable__Index__Logic;
module Components = Measurable__Index__Components;

module SearchResultsA = {
  type query = {
    key: string,
    value: string,
  };
  type t = array(query);
};

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
      ~searchParams: string,
      ~loggedInUser: Context.Primary.User.t,
      ~itemsPerPage: int=20,
      ~layout,
    ) => {
  let state =
    Measurable__Index__Logic.SearchResults.fromString(searchParams).state
    |> E.O.fmap(r => r)
    |> E.O.default(`OPEN);
  let loadData = load3Queries(channelId, [|state|], itemsPerPage);
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
         measurablesStateStats,
       )
    |> layout
  );
};