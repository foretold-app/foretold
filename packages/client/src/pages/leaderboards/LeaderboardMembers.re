module ReducerConfig = {
  type itemType = Types.agentChannel;
  type callFnParams = (option(string), option(string));

  let getId = (e: itemType) => e.id;

  let onItemDeselected = _ => ();
  let onItemSelected = (_: option(itemType)) => ();

  let callFn = ((channelId, agentId): callFnParams) =>
    AgentChannelsGet.component(~channelId, ~agentId, ());

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

[@react.component]
let make =
    (
      ~channelId=None,
      ~agentId=None,
      ~head=Leaderboard.head(~subTab=ByMember),
      ~columns=LeaderboardTable.Columns.members,
      ~children=<Null />,
    ) => {
  let subComponent = (reducerParams: Reducer.state) => {
    let items =
      switch (reducerParams.response) {
      | Success(connection) => connection.edges
      | _ => [||]
      };

    let items = items |> E.A.fmap(Primary.LeaderboardItem.fromAgentChannel);

    let body =
      switch (reducerParams.response) {
      | Success(_) =>
        Array.length(items) > 0
          ? <ForetoldComponents.PageCard.Body>
              <LeaderboardTable items columns />
            </ForetoldComponents.PageCard.Body>
          : <NothingToShow />
      | _ => <Spin />
      };

    let head =
      head(
        ~channelId,
        ~paginationPage=
          <Reducer.Components.PaginationPage state=reducerParams />,
        (),
      );

    <SLayout head container=`fluid> body </SLayout>;
  };

  <Reducer callFnParams=(channelId, agentId) subComponent />;
};