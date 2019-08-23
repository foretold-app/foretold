open Style.Grid;

module ReducerConfig = {
  type itemType = Types.measurement;
  type callFnParams = option(string);

  let getId = (e: itemType) => e.id;
  let callFn = (channelId: callFnParams) =>
    MeasurementsGet.componentWithAgent(~agentId="");

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("Leaderboard");
type pageParams = {id: string};

let make =
    (
      ~channelId: option(string)=None,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let pagination = (reducerParams: Reducer.Types.reducerParams) =>
      <Div>
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

    let subComponent = (reducerParams: Reducer.Types.reducerParams) => {
      let items =
        switch (reducerParams.response) {
        | Success(connection) => connection.edges
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

    <Reducer itemsPerPage=20 callFnParams=channelId subComponent />;
  },
};