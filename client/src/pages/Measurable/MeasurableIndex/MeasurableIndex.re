open Utils;
open Foretold__GraphQL;

module Types = Measurable__Index__Types;
module Components = Measurable__Index__Components;
open Types.Redux;

let component = ReasonReact.reducerComponent("MeasurableIndex");

let itemsPerPage = 20;

let load3Queries = (channelId, page, itemsPerPage, fn) =>
  ((a, b, c) => E.HttpResponse.merge3(a, b, c) |> fn)
  |> E.F.flatten3Callbacks(
       Queries.Channel.component2(~id=channelId),
       Queries.SeriesCollection.component2,
       Queries.Measurables.component2(channelId, page, itemsPerPage),
     );

let make =
    (~channelId: string, ~loggedInUser: Context.Primary.User.t, _children) => {
  ...component,
  initialState: () => {page: 0, selectedIndex: None},
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
    loadData(data =>
      Types.MeasurableIndexDataState.make({
        page: state.page,
        pageSelectedIndex: None,
        loggedInUser,
        query: data,
      })
      |> Components.MeasurableIndexDataState.toComponent(send)
    );
  },
};