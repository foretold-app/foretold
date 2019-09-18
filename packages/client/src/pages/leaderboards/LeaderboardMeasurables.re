module ReducerConfig = {
  type itemType = Types.agentMeasurable;
  type callFnParams = option(string);

  let getId = (e: itemType) => e.id;
  let callFn = (channelId: callFnParams) =>
    AgentMeasurablesGet.component(~channelId, ());

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("LeaderboardMeasurables");
let make = (~channelId=None, _children) => {
  ...component,
  render: _ => {
    let subComponent = (reducerParams: Reducer.Types.reducerParams) => {
      let items =
        switch (reducerParams.response) {
        | Success(connection) => connection.edges
        | _ => [||]
        };

      let items =
        items |> E.A.fmap(Primary.LeaderboardItem.fromAgentMeasurable);

      let body =
        switch (reducerParams.response) {
        | Success(_) =>
          Array.length(items) > 0
            ? <FC.PageCard.Body>
                <LeaderboardTable.Jsx2
                  items
                  columns=LeaderboardTable.Columns.measurables
                />
              </FC.PageCard.Body>
            : <NothingToShow />
        | _ => <Spin />
        };

      let head =
        <Leaderboard.Pagination
          channelId
          paginationPage={Reducer.Components.paginationPage(reducerParams)}
          subTab=ByMeasurable
        />;

      <SLayout head> body </SLayout>;
    };

    <Reducer callFnParams=channelId subComponent />;
  },
};