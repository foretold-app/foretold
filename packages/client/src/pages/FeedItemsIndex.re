open Style.Grid;

module ReducerConfig = {
  type itemType = Types.feedItem;
  type callFnParams = (option(string), option(string));

  let getId = (e: itemType) => e.id;

  let onItemDeselected = _ => ();
  let onItemSelected = (_: option(itemType)) => ();

  let callFn = ((channelId, agentId): callFnParams) =>
    FeedItemsGet.component2(~channelId, ~agentId);

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

[@react.component]
let make = (~channelId=None, ~agentId=None) => {
  let pagination = (reducerParams: Reducer.reducerParams) =>
    <Div>
      <Div
        float=`right
        styles=[
          Css.style([
            ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
            ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {Reducer.Components.paginationPage(reducerParams)}
      </Div>
    </Div>;

  let subComponent = (reducerParams: Reducer.reducerParams) => {
    let feedItems =
      switch (reducerParams.response) {
      | Success(connection) => connection.edges
      | _ => [||]
      };

    let table =
      switch (channelId) {
      | Some("")
      | None => <FeedItemsTable feedItems />
      | _ => <FeedItemsTable feedItems columns=FeedItemsTable.Columns.short />
      };

    let isFound = Array.length(feedItems) > 0;

    let body =
      switch (reducerParams.response) {
      | Success(_) =>
        isFound
          ? <ForetoldComponents.PageCard.Body>
              table
            </ForetoldComponents.PageCard.Body>
          : <NothingToShow />
      | _ => <Spin />
      };

    let head = isFound ? pagination(reducerParams) : <Null />;

    <SLayout head container=`fluid> body </SLayout>;
  };

  <Reducer itemsPerPage=40 callFnParams=(channelId, agentId) subComponent />;
};