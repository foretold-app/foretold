open Style.Grid;

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

let subTab = (state, text, num: int, isActive) =>
  <FC.Tab2
    isActive
    number=num
    onClick={LinkType.onClick(
      Relative(
        MeasurableQueryIndex.make(Some(state))
        |> MeasurableQueryIndex.toUrlParams,
      ),
    )}>
    {text |> Utils.ste}
  </FC.Tab2>;

let pagination = (reducerParams: Reducer.Types.reducerParams) =>
  <Div>
    <Div float=`left>
      <Div
        float=`left
        styles=[
          Css.style([
            FC.PageCard.HeaderRow.Styles.itemTopPadding,
            FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {subTab([|`OPEN|], "Open", 1, true)}
        {subTab([|`OPEN|], "Another", 10, false)}
      </Div>
    </Div>
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

let component = ReasonReact.statelessComponent("Leaderboard");
let make =
    (
      ~channelId: option(string)=None,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let subComponent = (reducerParams: Reducer.Types.reducerParams) => {
      let items =
        switch (reducerParams.response) {
        | Success(connection) =>
          connection.edges
          |> E.A.fmap(node => Primary.LeaderboardItem.fromMeasurement(node))
        | _ => [||]
        };

      let table = <LeaderboardTable.Jsx2 items />;

      let isFound = Array.length(items) > 0;

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
      |> layout;
    };

    <Reducer callFnParams=channelId subComponent />;
  },
};