open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;
open Rationale.Option.Infix;

module type Config = {
  type itemType;
  let isEqual: (itemType, itemType) => bool;
  let getId: itemType => string;
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

  module Components = {
    open Reducers.ReducerParams;

    let deselectButton = send =>
      SLayout.channelBack(~onClick=_ => send(Types.Deselect), ());

    module Styles = {
      open Css;
      let header = (~isDisabled) => {
        let normalStyles = [
          fontSize(`em(1.3)),
          float(`left),
          padding(`px(6)),
          lineHeight(`px(0)),
          marginRight(`em(0.7)),
          marginTop(`em(0.1)),
          color(`hex("e6e5e5")),
          borderRadius(`percent(50.)),
          userSelect(`none),
        ];
        let enabledOnlyStyles = [
          color(`hex("a3abb6")),
          cursor(`pointer),
          selector(
            ":hover",
            [color(`hex("445b7d")), background(`hex("e9eff7"))],
          ),
        ];
        let allStyles =
          isDisabled ?
            normalStyles : E.L.append(normalStyles, enabledOnlyStyles);
        style(allStyles);
      };
    };

    let pageButton' = (facesRight: bool, action, canMove, params) =>
      <div
        className={Styles.header(~isDisabled=!canMove(params))}
        onClick={_ => params.send(action)}
        disabled={!canMove(params)}>
        <Icon.Icon icon={facesRight ? "CHEVRON_RIGHT" : "CHEVRON_LEFT"} />
      </div>;

    type buttonType =
      | PageLast
      | PageNext
      | ItemLast
      | ItemNext;

    let pageButton' = (buttonType: buttonType) =>
      switch (buttonType) {
      | PageLast => pageButton'(false, Types.LastPage, canDecrementPage)
      | PageNext => pageButton'(true, Types.NextPage, canIncrementPage)
      | ItemLast =>
        pageButton'(false, Types.LastSelection, canDecrementSelection)
      | ItemNext =>
        pageButton'(true, Types.NextSelection, canIncrementSelection)
      };

    type buttonGroupType =
      | Page
      | Item;
    let buttonDuo = (buttonGroupType: buttonGroupType, params) =>
      switch (buttonGroupType) {
      | Page =>
        <>
          {pageButton'(PageLast, params)}
          {pageButton'(PageNext, params)}
        </>
      | Item =>
        <>
          {pageButton'(ItemLast, params)}
          {pageButton'(ItemNext, params)}
        </>
      };

    let correctButtonDuo = params =>
      switch (params.selection) {
      | Some(_) => buttonDuo(Item, params)
      | None => buttonDuo(Page, params)
      };

    let findIndexOfId = (t: Types.reducerParams, id: string) =>
      switch (t.response) {
      | Success(m) => m |> E.A.findIndex(r => Config.getId(r) == id)
      | _ => None
      };

    let selectItemAction = (t: t, id) =>
      findIndexOfId(t, id) |> E.O.fmap(e => Types.SelectIndex(e));

    let sendSelectItem = (t: t, id) =>
      selectItemAction(t, id) |> E.O.fmap(t.send) |> E.O.default();
  };

  let component = ReasonReact.reducerComponent("SelectWithPaginationReducer");

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