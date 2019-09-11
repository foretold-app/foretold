module ReducerConfig = {
  type itemType = Types.agentChannel;
  type callFnParams = option(string);

  let getId = (e: itemType) => e.id;
  let callFn = (channelId: callFnParams) =>
    AgentChannelsGet.component(~channelId, ());

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("LeaderboardChannels");
let make =
    (
      ~channelId: option(string)=None,
      ~subTab: Routing.ChannelPage.leaderboard,
      _children,
    ) => {
  ...component,
  render: _ => {
    let subComponent = (reducerParams: Reducer.Types.reducerParams) => {
      let items =
        switch (reducerParams.response) {
        | Success(connection) => connection.edges
        | _ => [||]
        };

      let items =
        items
        |> E.A.fmap(node => Primary.LeaderboardItem.fromAgentChannel(node));

      let body =
        switch (reducerParams.response) {
        | Success(_) =>
          Array.length(items) > 0
            ? <Fc.FC.PageCard.Body>
                <LeaderboardTable.Jsx2
                  items
                  columns=LeaderboardTable.Columns.members
                />
              </Fc.FC.PageCard.Body>
            : <SLayout.NothingToShow />
        | _ => <SLayout.Spin />
        };

      let head =
        Leaderboard.pagination(
          channelId,
          Reducer.Components.paginationPage(reducerParams),
          subTab,
        );

      SLayout.LayoutConfig.make(~head, ~body) |> SLayout.FullPage.makeWithEl;
    };

    <Reducer callFnParams=channelId subComponent />;
  },
};