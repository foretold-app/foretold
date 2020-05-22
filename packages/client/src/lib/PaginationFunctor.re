open Rationale.Option.Infix;

type direction = Primary.Connection.direction;

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
      isDisabled ? normalStyles : E.L.append(normalStyles, enabledOnlyStyles);

    style(allStyles);
  };
};

module type Config = {
  type itemType;
  type callFnParams;

  let isEqual: (itemType, itemType) => bool;
  let getId: itemType => string;

  let onItemDeselected: callFnParams => unit;
  let onItemSelected: option(itemType) => unit;

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
  type item =
    | ItemUnselected
    | ItemDeselected
    | ItemSelected(int);

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

  type state = {
    itemsPerPage: int,
    item,
    response,
    selection: option(Config.itemType),
    send,
    direction,
  };

  let nextSelection = (itemsPerPage: int, itemSelected: int) =>
    E.BoundedInt.increment(itemSelected, itemsPerPage)
    <$> (selectedIndex => ItemSelected(selectedIndex));

  let lastSelection = (itemsPerPage: int, itemSelected: int) =>
    E.BoundedInt.decrement(itemSelected, itemsPerPage)
    <$> (selectedIndex => ItemSelected(selectedIndex));

  let selection = (item: item, response: response) =>
    switch (item, response) {
    | (ItemSelected(selectedIndex), Success(m)) =>
      E.A.get(m.edges, selectedIndex)
    | _ => None
    };

  let pageIndex = (state: state) =>
    switch (state.item) {
    | ItemSelected(selectedIndex) => Some(selectedIndex)
    | ItemUnselected(_) => None
    };

  let canDecrementPage = (state: state) =>
    state.response
    |> HttpResponse.fmap((r: connection) =>
         Primary.Connection.hasPreviousPage(r)
       )
    |> HttpResponse.flattenDefault(false, a => a);

  let canIncrementPage = (state: state) =>
    state.response
    |> HttpResponse.fmap((r: connection) =>
         Primary.Connection.hasNextPage(r)
       )
    |> HttpResponse.flattenDefault(false, a => a);

  let itemExistsAtIndex = (state: state, index) =>
    switch (state.response) {
    | Success(r) => index < E.A.length(r.edges) && index >= 0
    | _ => false
    };

  let canDecrementSelection = (state: state) =>
    state
    |> pageIndex
    |> E.O.fmap(r => itemExistsAtIndex(state, r - 1))
    |> E.O.default(false);

  let canIncrementSelection = (state: state) =>
    state
    |> pageIndex
    |> E.O.fmap(r => itemExistsAtIndex(state, r + 1))
    |> E.O.default(false);

  let totalItems = (state: state) =>
    switch (state.response) {
    | Success(m) => m.total
    | _ => None
    };

  let lowerBoundIndex = (state: state) =>
    switch (state.response) {
    | Success(m) => m.pageInfo.startCursor |> E.O.fmap(Primary.Cursor.toInt)
    | _ => None
    };

  let upperBoundIndex = (state: state) =>
    switch (state.response) {
    | Success(m) => m.pageInfo.endCursor |> E.O.fmap(Primary.Cursor.toInt)
    | _ => None
    };

  let selectionIndex = (state: state) =>
    switch (pageIndex(state), lowerBoundIndex(state)) {
    | (Some(page), Some(lower)) => Some(page + lower)
    | _ => None
    };

  let changePage = (response, direction, pageDirection): direction =>
    response
    |> HttpResponse.fmap((r: Primary.Connection.t('a)) =>
         pageDirection(r) |> E.O.fmap(d => d)
       )
    |> HttpResponse.flattenDefault(None, a => a)
    |> E.O.default(direction);

  let nextPage = (response, direction): direction =>
    changePage(response, direction, Primary.Connection.nextPageDirection);

  let lastPage = (response, direction): direction =>
    changePage(response, direction, Primary.Connection.lastPageDirection);

  let selectIndex = (i, itemsPerPage) =>
    E.BoundedInt.make(i, itemsPerPage)
    <$> (selectedIndex => ItemSelected(selectedIndex));

  let findIndexOfId = (state: state, id: string): option(int) =>
    switch (state.response) {
    | Success(m) => m.edges |> E.A.findIndex(r => Config.getId(r) == id)
    | _ => None
    };

  let selectItemAction = (state, id: string): option(action) => {
    findIndexOfId(state, id) |> E.O.fmap(e => SelectIndex(e));
  };

  let sendSelectItem = (state: state, id: string): unit => {
    selectItemAction(state, id) |> E.O.fmap(state.send) |> E.O.default();
    ();
  };

  module Components = {
    module DeselectButton = {
      [@react.component]
      let make = (~send) => {
        <SLayout.ChannelBack onClick={_ => send(Deselect)} />;
      };
    };

    module PageButton = {
      [@react.component]
      let make = (~facesRight: bool, ~action: action, ~canMove, ~state: state) => {
        <div
          className={Styles.header(~isDisabled=!canMove(state))}
          onClick={_ => canMove(state) ? state.send(action) : ()}
          disabled={!canMove(state)}>
          <Icon icon={facesRight ? "CHEVRON_RIGHT" : "CHEVRON_LEFT"} />
        </div>;
      };
    };

    module CorrectButtonDuo = {
      [@react.component]
      let make = (~state: state) =>
        switch (state.selection) {
        | Some(_) =>
          <>
            <PageButton
              facesRight=false
              action=LastSelection
              canMove=canDecrementSelection
              state
            />
            <PageButton
              facesRight=true
              action=NextSelection
              canMove=canIncrementSelection
              state
            />
          </>
        | None =>
          <>
            <PageButton
              facesRight=false
              action=LastPage
              canMove=canDecrementPage
              state
            />
            <PageButton
              facesRight=true
              action=NextPage
              canMove=canIncrementPage
              state
            />
          </>
        };
    };

    module PaginationItem = {
      [@react.component]
      let make = (~state) =>
        switch (totalItems(state), selectionIndex(state)) {
        | (Some(count), Some(selection)) =>
          ForetoldComponents.PaginationButtons.make({
            currentValue: Item(selection + 1),
            max: count,
            pageLeft: {
              isDisabled: !canDecrementSelection(state),
              onClick: _ => state.send(LastSelection),
            },
            pageRight: {
              isDisabled: !canIncrementSelection(state),
              onClick: _ => state.send(NextSelection),
            },
          })
        | _ => "" |> Utils.ste
        };
    };

    module PaginationPage = {
      [@react.component]
      let make = (~state) =>
        switch (
          totalItems(state),
          upperBoundIndex(state),
          lowerBoundIndex(state),
        ) {
        | (Some(count), Some(upper), Some(lower)) =>
          ForetoldComponents.PaginationButtons.make({
            currentValue: Range(lower + 1, upper + 1),
            max: count,
            pageLeft: {
              isDisabled: !canDecrementPage(state),
              onClick: _ => state.send(LastPage),
            },
            pageRight: {
              isDisabled: !canIncrementPage(state),
              onClick: _ => state.send(NextPage),
            },
          })
        | _ => "" |> Utils.ste
        };
    };
  };

  let compareItems =
      (
        connectionA: Primary.Connection.t('a),
        connectionB: Primary.Connection.t('a),
      ) =>
    Belt.Array.eq(connectionA.edges, connectionB.edges, Config.isEqual);

  [@react.component]
  let make =
      (~itemsPerPage=20, ~callFnParams: Config.callFnParams, ~subComponent) => {
    let (item, setItem) = React.useState(() => ItemUnselected);

    let (response, setResponse) = React.useState(() => HttpResponse.Loading);

    let (direction, setDirection) =
      React.useState(() => Primary.Connection.NoneDirection);

    let send = action =>
      switch (item, action) {
      | (ItemUnselected | ItemDeselected, NextPage) =>
        setDirection(_ => nextPage(response, direction))

      | (ItemUnselected | ItemDeselected, LastPage) =>
        setDirection(_ => lastPage(response, direction))

      | (ItemUnselected | ItemDeselected, SelectIndex(i)) =>
        selectIndex(i, itemsPerPage)
        |> E.O.fmap(item => setItem(_ => item))
        |> ignore

      | (ItemSelected(_), Deselect) => setItem(_ => ItemDeselected)

      | (ItemSelected(itemSelected), NextSelection) =>
        nextSelection(itemsPerPage, itemSelected)
        |> E.O.fmap(item => setItem(_ => item))
        |> ignore

      | (ItemSelected(itemSelected), LastSelection) =>
        lastSelection(itemsPerPage, itemSelected)
        |> E.O.fmap(item => setItem(_ => item))
        |> ignore

      | _ => ()
      };

    React.useEffect(() => {
      switch (item) {
      | ItemDeselected => Config.onItemDeselected(callFnParams)
      | ItemSelected(_) => Config.onItemSelected(selection(item, response))
      | _ => ()
      };
      None;
    });

    let innerComponentFn = freshResponse => {
      // @todo: Fix this. Use hooks somehow!
      if (!HttpResponse.isEq(response, freshResponse, compareItems)) {
        setResponse(_ => freshResponse);
      };

      subComponent({
        itemsPerPage,
        item,
        response: freshResponse,
        selection: selection(item, response),
        send,
        direction,
      });
    };

    Config.callFn(
      callFnParams,
      ~direction,
      ~pageLimit=Js.Json.number(itemsPerPage |> float_of_int),
      ~innerComponentFn,
    );
  };
};