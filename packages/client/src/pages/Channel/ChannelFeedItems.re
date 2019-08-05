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
    Reducer.make(
      ~itemsPerPage=20,
      ~callFnParams=channelId,
      ~subComponent=selectWithPaginationParams =>
      SLayout.LayoutConfig.make(
        ~head=
          switch (selectWithPaginationParams.selection) {
          | Some(_selection) =>
            <>
              {Reducer.Components.deselectButton(
                 selectWithPaginationParams.send,
               )}
              {Reducer.Components.correctButtonDuo(selectWithPaginationParams)}
            </>
          | None => <div />
          },
        ~body=
          switch (
            selectWithPaginationParams.response,
            selectWithPaginationParams.selection,
          ) {
          | (_, Some(_)) => "Selected something" |> Utils.ste

          | (Success(connection), None) =>
            let feedItems = connection.edges;

            <FeedItemsTable.Jsx2 feedItems />;
          | _ => <SLayout.Spin />
          },
      )
      |> layout
    )
    |> E.React.makeToEl;
  },
};