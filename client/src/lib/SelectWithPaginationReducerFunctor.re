open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;
open Rationale.Option.Infix;

module type Config = {
  type itemType;
  let isEqual: (itemType, itemType) => bool;
  type callFnParams = string;
  let callFn:
    (
      callFnParams,
      ~pageNumber: int,
      ~pageLimit: int,
      ~innerComponentFn: Client.E.HttpResponse.t(array(itemType)) =>
                         ReasonReact.reactElement
    ) =>
    ReasonReact.reactElement;
};

module Make = (Config: Config) => {
  module Config = Config;
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

    type response = Client.E.HttpResponse.t(array(Config.itemType));

    type action =
      | UpdateResponse(response)
      | NextPage
      | LastPage
      | Deselect
      | SelectIndex(int)
      | NextSelection
      | LastSelection;

    let reducerName = action =>
      switch (action) {
      | NextPage => "NextPage"
      | LastPage => "LastPage"
      | Deselect => "Deselect"
      | SelectIndex(int) => "SelectIndex"
      | NextSelection => "nextSelection"
      | LastSelection => "LastSelection"
      | _ => "Other"
      };

    type send = action => unit;

    type reducerParams = {
      itemsPerPage: int,
      itemState,
      response,
      selection: option(Config.itemType),
      send,
    };

    type state = {
      itemState,
      response,
    };
  };

  open Types;

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
    };

    module State = {
      type t = state;
      let selection = (t: t) =>
        switch (t.itemState, t.response) {
        | (ItemSelected({selectedIndex}), Success(m)) =>
          E.A.get(m, selectedIndex)
        | _ => None
        };
    };

    module ReducerParams = {
      type t = reducerParams;

      let pageNumber = (t: t) =>
        switch (t.itemState) {
        | ItemSelected(r) => r.pageNumber
        | ItemUnselected(r) => r.pageNumber
        };

      let pageIndex = (t: t) =>
        switch (t.itemState) {
        | ItemSelected(r) => Some(r.selectedIndex)
        | ItemUnselected(_) => None
        };

      let canDecrementPage = (t: t) => t |> pageNumber > 0;

      let canIncrementPage = (t: t) =>
        switch (t.response) {
        | Success(r) => r |> E.A.length == t.itemsPerPage
        | _ => false
        };

      let itemExistsAtIndex = (t: t, index) =>
        switch (t.response) {
        | Success(r) => index < E.A.length(r) && index >= 0
        | _ => false
        };

      let canDecrementSelection = (t: t) =>
        t
        |> pageIndex
        |> E.O.fmap(r => itemExistsAtIndex(t, r - 1))
        |> E.O.default(false);

      let canIncrementSelection = (t: t) =>
        t
        |> pageIndex
        |> E.O.fmap(r => itemExistsAtIndex(t, r + 1))
        |> E.O.default(false);
    };
  };

  let component = ReasonReact.reducerComponent("PaginationReducer");

  let compareItems = (a, b) => Belt.Array.eq(a, b, Config.isEqual);

  let make =
      (
        ~itemsPerPage=20,
        ~callFnParams: Config.callFnParams,
        ~subComponent,
        _children,
      ) => {
    ...component,
    initialState: () => {
      itemState: ItemUnselected({pageNumber: 0}),
      response: Loading,
    },
    reducer: (action, state: state) =>
      switch (action) {
      | UpdateResponse(response) =>
        {response, itemState: state.itemState}->ReasonReact.Update
      | _ =>
        let newState =
          switch (state, action) {
          | ({itemState: ItemUnselected(s)}, _) =>
            Reducers.ItemUnselected.newState(itemsPerPage, s, action)
          | ({itemState: ItemSelected(s)}, _) =>
            Reducers.ItemSelected.newState(itemsPerPage, s, action)
          };
        switch (newState) {
        | Some(s) =>
          {response: state.response, itemState: s}->ReasonReact.Update
        | None => ReasonReact.NoUpdate
        };
      },
    render: ({state, send}) =>
      callFnParams
      |> Config.callFn(
           ~pageNumber=state.itemState |> Reducers.ItemState.pageNumber,
           ~pageLimit=itemsPerPage,
           ~innerComponentFn=response => {
             if (!E.HttpResponse.isEq(state.response, response, compareItems)) {
               send(UpdateResponse(response));
             };
             subComponent({
               itemsPerPage,
               itemState: state.itemState,
               response: state.response,
               selection: Reducers.State.selection(state),
               send,
             });
           },
         ),
  };
};