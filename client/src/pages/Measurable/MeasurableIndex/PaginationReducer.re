open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;
open Rationale.Option.Infix;

module Types = {
  type pageNumber = int;

  type itemUnselected = {pageNumber: int};

  type itemSelected = {
    pageNumber,
    selectedIndex: int,
  };

  type itemState =
    | ItemUnselected(itemUnselected)
    | ItemSelected(itemSelected);

  type action =
    | NextPage
    | LastPage
    | Deselect
    | SelectIndex(int)
    | NextSelection
    | LastSelection;

  type reducerParams = {
    itemsPerPage: int,
    itemState,
    send: action => unit,
  };
};

open Types;

module Reducers = {
  module ItemSelected = {
    type t = itemSelected;

    let deselect = (t: t) => ItemUnselected({pageNumber: t.pageNumber});

    let nextSelection = (t: t) =>
      E.BoundedInt.increment(t.selectedIndex, t.pageNumber)
      <$> (
        selectedIndex =>
          ItemSelected({pageNumber: t.pageNumber, selectedIndex})
      );

    let lastSelection = (t: t) =>
      E.BoundedInt.decrement(t.selectedIndex, t.pageNumber)
      <$> (
        selectedIndex =>
          ItemSelected({pageNumber: t.pageNumber, selectedIndex})
      );

    let newState = (t: t, action: action) =>
      (
        switch (action) {
        | Deselect => Some(deselect ||> E.O.some)
        | NextSelection => Some(nextSelection)
        | LastSelection => Some(lastSelection)
        | _ => None
        }
      )
      |> E.O.flatApply(_, t);
  };

  module ItemUnselected = {
    type t = itemUnselected;

    let nextPage = (t: t) =>
      t.pageNumber
      |> E.NonZeroInt.increment
      <$> (p => ItemUnselected({pageNumber: p}));

    let lastPage = (t: t) =>
      t.pageNumber
      |> E.NonZeroInt.decrement
      <$> (p => ItemUnselected({pageNumber: p}));

    let selectIndex = (i, t: t) =>
      E.BoundedInt.make(i, t.pageNumber)
      <$> (s => ItemSelected({pageNumber: t.pageNumber, selectedIndex: s}));

    let newState = (t: t, action: action) =>
      (
        switch (action) {
        | NextPage => Some(nextPage)
        | LastPage => Some(lastPage)
        | SelectIndex(i) => Some(selectIndex(i))
        | _ => None
        }
      )
      |> E.O.flatApply(_, t);
  };
};

let component = ReasonReact.reducerComponent("PaginationReducer");

let make = (~itemsPerPage=20, ~subComponent, _children) => {
  ...component,
  initialState: () => ItemUnselected({pageNumber: 0}),
  reducer: (action, state) => {
    let newState =
      switch (state) {
      | ItemUnselected(s) => Reducers.ItemUnselected.newState(s, action)
      | ItemSelected(s) => Reducers.ItemSelected.newState(s, action)
      };
    switch (newState) {
    | Some(s) => s->ReasonReact.Update
    | None => ReasonReact.NoUpdate
    };
  },
  render: ({state, send}) =>
    subComponent(
      ~paginationReducerParams={itemsPerPage, itemState: state, send},
    ),
};