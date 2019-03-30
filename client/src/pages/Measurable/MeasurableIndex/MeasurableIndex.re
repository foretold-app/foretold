open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;

module Types = Measurable__Index__Logic;
module Components = Measurable__Index__Components;

let load3Queries = (channelId, itemsPerPage, fn) =>
  ((a, b, c) => (a, b, c) |> fn)
  |> E.F.flatten3Callbacks(
       Types.SelectWithPaginationReducer.make(
         ~itemsPerPage,
         ~callFnParams=channelId,
         ~subComponent=_,
       ),
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
      ~itemsPerPage: int=20,
      ~layout=SLayout.FullPage.makeWithEl,
    ) => {
  let loadData = load3Queries(channelId, itemsPerPage);
  loadData(((selectWithPaginationParams, channel, query)) =>
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