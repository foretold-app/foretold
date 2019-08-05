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
    let subComponent = selectWithPaginationParams =>
      SLayout.LayoutConfig.make(
        ~head=
          <Div>
            <Div
              float=`right
              styles=[
                Css.style([
                  FC.PageCard.HeaderRow.Styles.itemTopPadding,
                  FC.PageCard.HeaderRow.Styles.itemBottomPadding,
                ]),
              ]>
              {Reducer.Components.paginationPage(selectWithPaginationParams)}
            </Div>
          </Div>,
        ~body=
          switch (
            selectWithPaginationParams.response,
            selectWithPaginationParams.selection,
          ) {
          | (Success(connection), _) =>
            let feedItems = connection.edges;

            <FC.PageCard.Body>
              <FeedItemsTable.Jsx2 feedItems />
            </FC.PageCard.Body>;
          | _ => <SLayout.Spin />
          },
      )
      |> layout;

    <Reducer itemsPerPage=1 callFnParams=channelId subComponent />;
  },
};