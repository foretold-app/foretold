open Utils;
open Rationale.Option.Infix;

type direction = Primary.Connection.direction;

module type Config = {
  type itemType;
  type callFnParams;

  let isEqual: (itemType, itemType) => bool;
  let getId: itemType => string;

  let callFn:
    (
      callFnParams,
      ~pageLimit: Js.Json.t,
      ~direction: direction,
      ~innerComponentFn: HttpResponse.t(Primary.Connection.t(itemType)) =>
                         ReasonReact.reactElement
    ) =>
    ReasonReact.reactElement;
};

module Make = (Config: Config) => {
  module Types = {
    type pageConfig = {direction};

    type itemSelected = {selectedIndex: int};

    type itemState =
      | ItemUnselected
      | ItemSelected(itemSelected);

    type connection = Primary.Connection.t(Config.itemType);

    type response = HttpResponse.t(connection);

    type action =
      | UpdateResponse(response)
      | NextPage
      | LastPage
      | Deselect
      | SelectIndex(int)
      | NextSelection
      | LastSelection;

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
      pageConfig,
    };
  };

  module Reducers = {
    module ItemSelected = {
      type t = Types.itemSelected;

      let deselect = (_, _) => Types.ItemUnselected;

      let nextSelection = (itemsPerPage: int, itemSelected: t) =>
        E.BoundedInt.increment(itemSelected.selectedIndex, itemsPerPage)
        <$> (
          selectedIndex => Types.ItemSelected({selectedIndex: selectedIndex})
        );

      let lastSelection = (itemsPerPage: int, itemSelected: t) =>
        E.BoundedInt.decrement(itemSelected.selectedIndex, itemsPerPage)
        <$> (
          selectedIndex => Types.ItemSelected({selectedIndex: selectedIndex})
        );

      let newState =
          (itemsPerPage: int, itemSelected: t, action: Types.action) =>
        (
          switch (action) {
          | Deselect => Some((a, b) => deselect(a, b) |> E.O.some)
          | NextSelection => Some(nextSelection)
          | LastSelection => Some(lastSelection)
          | _ => None
          }
        )
        |> (
          e =>
            switch (e) {
            | Some(fn) => fn(itemsPerPage, itemSelected)
            | None => None
            }
        );
    };

    module State = {
      type t = Types.state;

      let selection = (state: t) =>
        switch (state.itemState, state.response) {
        | (ItemSelected({selectedIndex}), Success(m)) =>
          E.A.get(m.edges, selectedIndex)
        | _ => None
        };
    };

    module ReducerParams = {
      type t = Types.reducerParams;

      let pageIndex = (reducerParams: t) =>
        switch (reducerParams.itemState) {
        | ItemSelected(r) => Some(r.selectedIndex)
        | ItemUnselected(_) => None
        };

      let canDecrementPage = (reducerParams: t) =>
        reducerParams.response
        |> HttpResponse.fmap((r: Types.connection) =>
             Primary.Connection.hasPreviousPage(r)
           )
        |> HttpResponse.flattenDefault(false, a => a);

      let canIncrementPage = (reducerParams: t) =>
        reducerParams.response
        |> HttpResponse.fmap((r: Types.connection) =>
             Primary.Connection.hasNextPage(r)
           )
        |> HttpResponse.flattenDefault(false, a => a);

      let itemExistsAtIndex = (reducerParams: t, index) =>
        switch (reducerParams.response) {
        | Success(r) => index < E.A.length(r.edges) && index >= 0
        | _ => false
        };

      let canDecrementSelection = (reducerParams: t) =>
        reducerParams
        |> pageIndex
        |> E.O.fmap(r => itemExistsAtIndex(reducerParams, r - 1))
        |> E.O.default(false);

      let canIncrementSelection = (reducerParams: t) =>
        reducerParams
        |> pageIndex
        |> E.O.fmap(r => itemExistsAtIndex(reducerParams, r + 1))
        |> E.O.default(false);

      let totalItems = (reducerParams: t) =>
        switch (reducerParams.response) {
        | Success(m) => m.total
        | _ => None
        };

      let lowerBoundIndex = (reducerParams: t) =>
        switch (reducerParams.response) {
        | Success(m) => m.pageInfo.startCursor |> E.O.fmap(int_of_string)
        | _ => None
        };

      let upperBoundIndex = (reducerParams: t) =>
        switch (reducerParams.response) {
        | Success(m) => m.pageInfo.endCursor |> E.O.fmap(int_of_string)
        | _ => None
        };

      let selectionIndex = (reducerParams: t) =>
        switch (pageIndex(reducerParams), lowerBoundIndex(reducerParams)) {
        | (Some(page), Some(lower)) => Some(page + lower)
        | _ => None
        };
    };

    module ItemUnselected = {
      open Types;
      let changePage = (state: Types.state, pageDirection) =>
        state.response
        |> HttpResponse.fmap((r: Primary.Connection.t('a)) =>
             pageDirection(r) |> E.O.fmap(d => {direction: d})
           )
        |> HttpResponse.flattenDefault(None, a => a)
        |> E.O.default(state.pageConfig);

      let nextPage = state =>
        changePage(state, Primary.Connection.nextPageDirection);

      let lastPage = (state): pageConfig =>
        changePage(state, Primary.Connection.lastPageDirection);

      let selectIndex = (i, itemsPerPage) =>
        E.BoundedInt.make(i, itemsPerPage)
        <$> (selectedIndex => ItemSelected({selectedIndex: selectedIndex}));

      let newState = (itemsPerPage, state, action) =>
        switch (action) {
        | NextPage => Some((ItemUnselected, nextPage(state)))
        | LastPage => Some((ItemUnselected, lastPage(state)))
        | SelectIndex(i) =>
          selectIndex(i, itemsPerPage)
          |> E.O.fmap(r => (r, state.pageConfig))
        | _ => None
        };
    };
  };

  module Components = {
    open Reducers.ReducerParams;

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
          color(`hex("838383")),
          cursor(`pointer),
          selector(
            ":hover",
            [color(`hex("445b7d")), background(`hex("e9eff7"))],
          ),
        ];

        let allStyles =
          isDisabled
            ? normalStyles : E.L.append(normalStyles, enabledOnlyStyles);

        style(allStyles);
      };
    };

    type buttonType =
      | PageLast
      | PageNext
      | ItemLast
      | ItemNext;

    type buttonGroupType =
      | Page
      | Item;

    let deselectButton = send =>
      SLayout.channelBack(~onClick=_ => send(Types.Deselect), ());

    let pageButton' =
        (facesRight: bool, action, canMove, params: Types.reducerParams) =>
      <div
        className={Styles.header(~isDisabled=!canMove(params))}
        onClick={_ => canMove(params) ? params.send(action) : ()}
        disabled={!canMove(params)}>
        <Icon.Icon icon={facesRight ? "CHEVRON_RIGHT" : "CHEVRON_LEFT"} />
      </div>;

    let pageButton' = buttonType =>
      switch (buttonType) {
      | PageLast => pageButton'(false, Types.LastPage, canDecrementPage)
      | PageNext => pageButton'(true, Types.NextPage, canIncrementPage)
      | ItemLast =>
        pageButton'(false, Types.LastSelection, canDecrementSelection)
      | ItemNext =>
        pageButton'(true, Types.NextSelection, canIncrementSelection)
      };

    let buttonDuo = (buttonGroupType, params) =>
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

    let correctButtonDuo = (params: Types.reducerParams) =>
      switch (params.selection) {
      | Some(_) => buttonDuo(Item, params)
      | None => buttonDuo(Page, params)
      };

    let paginationItem = reducerParams =>
      switch (totalItems(reducerParams), selectionIndex(reducerParams)) {
      | (Some(count), Some(selection)) =>
        FC.PaginationButtons.make({
          currentValue: Item(selection + 1),
          max: count,
          pageLeft: {
            isDisabled: !canDecrementSelection(reducerParams),
            onClick: _ => reducerParams.send(Types.LastSelection),
          },
          pageRight: {
            isDisabled: !canIncrementSelection(reducerParams),
            onClick: _ => reducerParams.send(Types.NextSelection),
          },
        })
      | _ => "" |> ste
      };

    let paginationPage = reducerParams =>
      switch (
        totalItems(reducerParams),
        upperBoundIndex(reducerParams),
        lowerBoundIndex(reducerParams),
      ) {
      | (Some(count), Some(upper), Some(lower)) =>
        FC.PaginationButtons.make({
          currentValue: Range(lower + 1, upper + 1),
          max: count,
          pageLeft: {
            isDisabled: !canDecrementPage(reducerParams),
            onClick: _ => reducerParams.send(Types.LastPage),
          },
          pageRight: {
            isDisabled: !canIncrementPage(reducerParams),
            onClick: _ => reducerParams.send(Types.NextPage),
          },
        })
      | _ => "" |> ste
      };

    let findIndexOfId =
        (reducerParams: Types.reducerParams, id: string): option(int) =>
      switch (reducerParams.response) {
      | Success(m) => m.edges |> E.A.findIndex(r => Config.getId(r) == id)
      | _ => None
      };

    let selectItemAction = (reducerParams, id: string): option(Types.action) => {
      findIndexOfId(reducerParams, id) |> E.O.fmap(e => Types.SelectIndex(e));
    };

    let sendSelectItem =
        (reducerParams: Types.reducerParams, id: string): unit => {
      selectItemAction(reducerParams, id)
      |> E.O.fmap(reducerParams.send)
      |> E.O.default();
      ();
    };
  };

  let compareItems =
      (
        connectionA: Primary.Connection.t('a),
        connectionB: Primary.Connection.t('a),
      ) =>
    Belt.Array.eq(connectionA.edges, connectionB.edges, Config.isEqual);

  open Types;
  let component = ReasonReact.reducerComponent("Pagination");
  let make =
      (
        ~itemsPerPage=20,
        ~callFnParams: Config.callFnParams,
        ~subComponent,
        _children,
      ) => {
    ...component,

    initialState: () => {
      itemState: ItemUnselected,
      response: Loading,
      pageConfig: {
        direction: None,
      },
    },

    reducer: (action, state: state) =>
      switch (action) {
      | UpdateResponse(response) =>
        ReasonReact.Update({
          response,
          itemState: state.itemState,
          pageConfig: state.pageConfig,
        })

      | _ =>
        let newState =
          switch (state) {
          | {itemState: ItemUnselected} =>
            Reducers.ItemUnselected.newState(itemsPerPage, state, action)

          | {itemState: ItemSelected(itemSelected)} =>
            Reducers.ItemSelected.newState(itemsPerPage, itemSelected, action)
            |> E.O.fmap(itemState => (itemState, state.pageConfig))
          };

        switch (newState) {
        | Some((itemState, pageConfig)) =>
          ReasonReact.Update({
            response: state.response,
            itemState,
            pageConfig,
          })

        | None => ReasonReact.NoUpdate
        };
      },

    render: ({state, send}) => {
      let innerComponentFn = response => {
        if (!HttpResponse.isEq(state.response, response, compareItems)) {
          send(UpdateResponse(response));
        };

        subComponent({
          itemsPerPage,
          itemState: state.itemState,
          response: state.response,
          selection: Reducers.State.selection(state),
          send,
        });
      };

      Config.callFn(
        callFnParams,
        ~direction=state.pageConfig.direction,
        ~pageLimit=Js.Json.number(itemsPerPage |> float_of_int),
        ~innerComponentFn,
      );
    },
  };
};
