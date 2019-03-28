open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;

module Types = Measurable__Index__Types;
module Components = Measurable__Index__Components;
open Types.Redux;

let component = ReasonReact.reducerComponent("MeasurableIndex");

let itemsPerPage = 20;

let load3Queries = (channelId, page, itemsPerPage, fn) =>
  ((a, b, c) => (a, E.HttpResponse.merge2(b, c)) |> fn)
  |> E.F.flatten3Callbacks(
       Queries.Channel.component2(~id=channelId),
       Queries.SeriesCollection.component2,
       Queries.Measurables.component2(channelId, page, itemsPerPage),
     );

let make =
    (
      ~channelId: string,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  initialState: () => Types.Redux.initialState,
  reducer: (action, state) =>
    switch (action) {
    | NextPage =>
      ReasonReact.Update({selectedIndex: None, page: state.page + 1})
    | LastPage =>
      ReasonReact.Update({selectedIndex: None, page: state.page - 1})
    | Select((num: option(int))) =>
      ReasonReact.Update({...state, selectedIndex: num})
    | SelectIncrement =>
      ReasonReact.Update({
        ...state,
        selectedIndex: state.selectedIndex |> E.O.fmap(E.I.increment),
      })
    | SelectDecrement =>
      ReasonReact.Update({
        ...state,
        selectedIndex: state.selectedIndex |> E.O.fmap(E.I.decrement),
      })
    },
  render: ({state, send}) => {
    let loadData = load3Queries(channelId, state.page, itemsPerPage);
    loadData(((channel, query)) =>
      Types.MeasurableIndexDataState.make({
        page: state.page,
        pageSelectedIndex: state.selectedIndex,
        loggedInUser,
        channel,
        query,
      })
      |> Components.MeasurableIndexDataState.toLayoutInput(send)
      |> layout
    );
  },
};