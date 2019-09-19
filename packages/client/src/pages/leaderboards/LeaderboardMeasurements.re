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
      ~channelId=None,
      ~head=Leaderboard.head(~subTab=ByMeasurement),
      ~columns=LeaderboardTable.Columns.default,
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

      let items = items |> E.A.fmap(Primary.LeaderboardItem.fromMeasurement);

      let body =
        switch (reducerParams.response) {
        | Success(_) =>
          Array.length(items) > 0
            ? <FC.PageCard.Body>
                <LeaderboardTable.Jsx2 items columns />
              </FC.PageCard.Body>
            : <NothingToShow />
        | _ => <Spin />
        };

      let head =
        head(
          ~channelId,
          ~paginationPage=Reducer.Components.paginationPage(reducerParams),
          (),
        );

      <SLayout head> body </SLayout>;
    };

    <Reducer callFnParams=channelId subComponent />;
  },
};