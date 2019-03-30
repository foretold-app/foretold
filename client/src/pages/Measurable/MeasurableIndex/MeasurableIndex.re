open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;

module Types = Measurable__Index__Logic;
module Components = Measurable__Index__Components;

let load2Queries = (channelId, itemsPerPage, fn) =>
  ((a, b) => (a, b) |> fn)
  |> E.F.flatten2Callbacks(
       Queries.Channel.component2(~id=channelId),
       Queries.SeriesCollection.component2(~channelId),
     );

module GetMeasurables = {
  type itemType = Context.Primary.Measurable.t;
  type callFnParams = string;
  let callFn = (e: callFnParams) =>
    Queries.Measurables.component2(~channelId=e);
  let isEqual = (a: itemType, b: itemType) => a.id == b.id;
};

let make =
    (
      ~channelId: string,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
    ) => {
  let loadData = load2Queries(channelId, 20);
  (
    selectWithPaginationParams =>
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
      )
  )
  |> Types.SelectWithPaginationReducer.make(
       ~itemsPerPage=20,
       ~callFnParams=channelId,
       ~subComponent=_,
     );
};