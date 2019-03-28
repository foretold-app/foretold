open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;

module Types = Measurable__Index__Types;
module Components = Measurable__Index__Components;

let load2Queries = (channelId, fn) =>
  ((a, b) => (a, b) |> fn)
  |> E.F.flatten2Callbacks(
       Queries.Channel.component2(~id=channelId),
       Queries.SeriesCollection.component2(~channelId),
     );

let make =
    (
      ~channelId: string,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      ~selectWithPaginationParams: SelectWithPaginationReducer.Types.reducerParams,
    ) => {
  let itemsPerPage = selectWithPaginationParams.itemsPerPage;
  let pageNumber =
    selectWithPaginationParams
    |> SelectWithPaginationReducer.Reducers.ReducerParams.pageNumber;
  let loadData = load2Queries(channelId);
  loadData(((channel, query)) =>
    Types.MeasurableIndexDataState.make({
      reducerParams: selectWithPaginationParams,
      loggedInUser,
      channel,
      query,
    })
    |> Components.MeasurableIndexDataState.toLayoutInput(
         selectWithPaginationParams.send,
       )
    |> layout
  );
};