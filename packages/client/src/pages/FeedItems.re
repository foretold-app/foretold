open Style.Grid;

module ReducerConfig = {
  type itemType = Types.feedItem;
  type callFnParams = (option(string), option(string));

  let getId = (e: itemType) => e.id;
  let callFn = ((channelId, agentId): callFnParams) =>
    FeedItemsGet.component2(~channelId, ~agentId);

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("FeedItems");
type pageParams = {id: string};

let make =
    (
      ~channelId: option(string)=None,
      ~agentId: option(string)=None,
      _children,
    ) => {
  ...component,
  render: _ => {
    let pagination = (reducerParams: Reducer.Types.reducerParams) =>
      <Div>
        <Div
          float=`right
          styles=[
            Css.style([
              FC.PageCard.HeaderRow.Styles.itemTopPadding,
              FC.PageCard.HeaderRow.Styles.itemBottomPadding,
            ]),
          ]>
          {Reducer.Components.paginationPage(reducerParams)}
        </Div>
      </Div>;

    let subComponent = (reducerParams: Reducer.Types.reducerParams) => {
      let feedItems =
        switch (reducerParams.response) {
        | Success(connection) => connection.edges
        | _ => [||]
        };

      let table =
        switch (channelId) {
        | Some("")
        | None => <FeedItemsTable.Jsx2 feedItems />
        | _ =>
          <FeedItemsTable.Jsx2
            feedItems
            columns=FeedItemsTable.Columns.short
          />
        };

      let isFound = Array.length(feedItems) > 0;

      let body =
        switch (reducerParams.response) {
        | Success(_) =>
          isFound
            ? <FC.PageCard.Body> table </FC.PageCard.Body>
            : <SLayout.NothingToShow />
        | _ => <SLayout.Spin />
        };

      SLayout.LayoutConfig.make(
        ~head=isFound ? pagination(reducerParams) : ReasonReact.null,
        ~body,
      )
      |> SLayout.FullPage.makeWithEl;
    };

    <Reducer
      itemsPerPage=20
      callFnParams=(channelId, agentId)
      subComponent
    />;
  },
};