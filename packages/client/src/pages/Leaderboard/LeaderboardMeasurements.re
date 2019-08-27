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

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("LeaderboardMeasurements");
let make =
    (
      ~channelId: option(string)=None,
      ~layout=SLayout.FullPage.makeWithEl,
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

      let isFound = Array.length(items) > 0;

      let body =
        switch (reducerParams.response) {
        | Success(_) =>
          isFound
            ? <FC.PageCard.Body>
                <LeaderboardTable.Jsx2 items />
              </FC.PageCard.Body>
            : <SLayout.NothingToShow />
        | _ => <SLayout.Spin />
        };

      SLayout.LayoutConfig.make(
        ~head=
          isFound
            ? Leaderboard.pagination(
                channelId,
                Reducer.Components.paginationPage(reducerParams),
                subTab,
              )
            : ReasonReact.null,
        ~body,
      )
      |> layout;
    };

    <Reducer callFnParams=channelId subComponent />;
  },
};