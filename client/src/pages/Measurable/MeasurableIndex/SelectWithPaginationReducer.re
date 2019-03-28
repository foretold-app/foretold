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

  let name = action =>
    switch (action) {
    | NextPage => "NextPage"
    | LastPage => "LastPage"
    | Deselect => "Deselect"
    | SelectIndex(int) => "SelectIndex"
    | NextSelection => "nextSelection"
    | LastSelection => "LastSelection"
    };

  type send = action => unit;

  type reducerParams = {
    itemsPerPage: int,
    itemState,
    send,
  };
};

open Types;

let itemsPerPage = 20;

module Reducers = {
  module ItemSelected = {
    type t = itemSelected;

    let deselect = (_, t: t) => ItemUnselected({pageNumber: t.pageNumber});

    let nextSelection = (itemsPerPage, t: t) =>
      E.BoundedInt.increment(t.selectedIndex, itemsPerPage)
      <$> (
        selectedIndex =>
          ItemSelected({pageNumber: t.pageNumber, selectedIndex})
      );

    let lastSelection = (itemsPerPage, t: t) =>
      E.BoundedInt.decrement(t.selectedIndex, itemsPerPage)
      <$> (
        selectedIndex =>
          ItemSelected({pageNumber: t.pageNumber, selectedIndex})
      );

    let newState = (itemsPerPage, t: t, action: action) =>
      (
        switch (action) {
        | Deselect => Some(((a, b) => deselect(a, b) |> E.O.some))
        | NextSelection => Some(nextSelection)
        | LastSelection => Some(lastSelection)
        | _ => None
        }
      )
      |> (
        e =>
          switch (e) {
          | Some(fn) => fn(itemsPerPage, t)
          | None => None
          }
      );
  };

  module ItemUnselected = {
    type t = itemUnselected;

    let nextPage = (_, t: t) =>
      t.pageNumber
      |> E.NonZeroInt.increment
      <$> (p => ItemUnselected({pageNumber: p}));

    let lastPage = (_, t: t) =>
      t.pageNumber
      |> E.NonZeroInt.decrement
      <$> (p => ItemUnselected({pageNumber: p}));

    let selectIndex = (i, itemsPerPage, t: t) =>
      E.BoundedInt.make(i, itemsPerPage)
      <$> (s => ItemSelected({pageNumber: t.pageNumber, selectedIndex: s}));

    let newState = (itemsPerPage, t: t, action: action) =>
      (
        switch (action) {
        | NextPage => Some(nextPage)
        | LastPage => Some(lastPage)
        | SelectIndex(i) => Some(selectIndex(i))
        | _ => None
        }
      )
      |> (
        e =>
          switch (e) {
          | Some(fn) => fn(itemsPerPage, t)
          | None => None
          }
      );
  };

  module ItemState = {
    type t = itemState;
    let pageNumber = (t: t) =>
      switch (t) {
      | ItemSelected(r) => r.pageNumber
      | ItemUnselected(r) => r.pageNumber
      };

    let pageIndex = (t: t) =>
      switch (t) {
      | ItemSelected(r) => Some(r.selectedIndex)
      | ItemUnselected(_) => None
      };
  };
};

let component = ReasonReact.reducerComponent("PaginationReducer");

let make = (~itemsPerPage=20, ~subComponent, _children) => {
  ...component,
  initialState: () => ItemUnselected({pageNumber: 0}),
  reducer: (action, state) => {
    Js.log3("HI", action |> Types.name, state);
    let newState =
      switch (state) {
      | ItemUnselected(s) =>
        Reducers.ItemUnselected.newState(itemsPerPage, s, action)
      | ItemSelected(s) =>
        Reducers.ItemSelected.newState(itemsPerPage, s, action)
      };
    switch (newState) {
    | Some(s) => s->ReasonReact.Update
    | None => ReasonReact.NoUpdate
    };
  },
  render: ({state, send}) =>
    subComponent(
      ~selectWithPaginationParams={itemsPerPage, itemState: state, send},
    ),
};