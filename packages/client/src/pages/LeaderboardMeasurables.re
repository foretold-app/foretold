open Style.Grid;

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

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

let pagination =
    (channelId: option(string), reducerParams: Reducer.Types.reducerParams) =>
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
        <FC.Tab2
          isActive=true
          onClick={LinkType.onClick(
            Internal(
              ChannelLeaderboard(channelId |> E.O.toString, ByMeasurement),
            ),
          )}>
          {"By Prediction" |> Utils.ste}
        </FC.Tab2>
        <FC.Tab2
          isActive=false
          onClick={LinkType.onClick(
            Internal(
              ChannelLeaderboard(channelId |> E.O.toString, ByMeasurable),
            ),
          )}>
          {"By Question" |> Utils.ste}
        </FC.Tab2>
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

let component = ReasonReact.statelessComponent("LeaderboardMeasurables");
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
          |> E.A.fmap(node =>
               Primary.LeaderboardItem.fromAgentMeasurable(node)
             )
        | _ => [||]
        };

      let table =
        <LeaderboardTable.Jsx2
          items
          columns=LeaderboardTable.Columns.measurables
        />;

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
        ~head=
          isFound ? pagination(channelId, reducerParams) : ReasonReact.null,
        ~body,
      )
      |> layout;
    };

    <Reducer callFnParams=channelId subComponent />;
  },
};