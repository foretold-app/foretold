open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;

module Types = Measurable__Index__Types;
module Components = Measurable__Index__Components;

let load3Queries = (channelId, page, itemsPerPage, fn) =>
  ((a, b, c) => (a, E.HttpResponse.merge2(b, c)) |> fn)
  |> E.F.flatten3Callbacks(
       Queries.Channel.component2(~id=channelId),
       Queries.SeriesCollection.component2(~channelId),
       Queries.Measurables.component2(channelId, page, itemsPerPage),
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
  let loadData = load3Queries(channelId, pageNumber, itemsPerPage);
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