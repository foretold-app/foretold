module ReducerConfig = {
  type itemType = Types.measurement;
  type callFnParams = option(string);

  let getId = (e: itemType) => e.id;
  let callFn = (channelId: callFnParams) =>
    MeasurementsGet.component(
      ~channelId,
      ~measurableState=Some([|Some(`JUDGED)|]),
      ~competitorType=Some([|`COMPETITIVE|]),
      (),
    );

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("LeaderboardMeasurements");
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
        |> E.A.fmap(node => Primary.LeaderboardItem.fromMeasurement(node));

      let body =
        switch (reducerParams.response) {
        | Success(_) =>
          Array.length(items) > 0
            ? <FC.PageCard.Body>
                <LeaderboardTable.Jsx2 items />
              </FC.PageCard.Body>
            : <SLayout.NothingToShow />
        | _ => <SLayout.Spin />
        };

      let head =
        <Leaderboard.Pagination
          channelId
          paginationPage={Reducer.Components.paginationPage(reducerParams)}
          subTab
        />;

      <SLayout head> body </SLayout>;
    };

    <Reducer callFnParams=channelId subComponent />;
  },
};