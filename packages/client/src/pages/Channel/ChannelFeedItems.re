open Style.Grid;

module ReducerConfig = {
  type itemType = Types.feedItem;
  type callFnParams = string;

  let getId = (e: itemType) => e.id;
  let callFn = (channelId: callFnParams) =>
    FeedItemsGet.component2(~channelId);

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("ChannelFeedItems");
type pageParams = {id: string};

let make =
    (~channelId: string, ~layout=SLayout.FullPage.makeWithEl, _children) => {
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

      let isFound = Array.length(feedItems) > 0;

      let body =
        switch (reducerParams.response) {
        | Success(_) =>
          isFound
            ? <FC.PageCard.Body>
                <FeedItemsTable.Jsx2 feedItems />
              </FC.PageCard.Body>
            : <SLayout.NothingToShow />
        | _ => <SLayout.Spin />
        };

      SLayout.LayoutConfig.make(
        ~head=isFound ? pagination(reducerParams) : ReasonReact.null,
        ~body,
      )
      |> layout;
    };

    <Reducer itemsPerPage=20 callFnParams=channelId subComponent />;
  },
};