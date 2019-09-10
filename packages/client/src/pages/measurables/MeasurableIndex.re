open Utils;
open Style.Grid;
open MeasurableIndex__Logic;

module ReducerParams = Reducer.Reducers.ReducerParams;

module LoadedAndSelected = {
  open MeasurableIndex__Logic.LoadedAndSelected;

  let header = (t: t, send: Reducer.Types.send) =>
    <>
      <Div
        float=`left
        styles=[
          Css.style([
            FC.PageCard.HeaderRow.Styles.itemTopPadding,
            FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {Reducer.Components.deselectButton(send)}
      </Div>
      <Div
        float=`right
        styles=[
          Css.style([
            FC.PageCard.HeaderRow.Styles.itemTopPadding,
            FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {Reducer.Components.paginationItem(t.reducerParams)}
      </Div>
    </>;

  let body = (t: t) =>
    <MeasurableFullPresentation
      id={t.selectedMeasurable.id}
      key={t.selectedMeasurable.id}
      loggedInUser={t.loggedInUser}
    />;

  let body2 = (t: t) =>
    <Measurements
      measurableId={t.selectedMeasurable.id}
      key={t.selectedMeasurable.id}
      loggedInUser={t.loggedInUser}
    />;
};

module LoadedAndUnselected = {
  open MeasurableIndex__Logic.LoadedAndUnselected;

  let stateLink = (state, text, num: int, isActive) =>
    <FC.Tab2
      isActive
      number=num
      onClick={LinkType.onClick(
        Relative(
          MeasurableQueryIndex.make(Some(state))
          |> MeasurableQueryIndex.toUrlParams,
        ),
      )}>
      {text |> ste}
    </FC.Tab2>;

  let header =
      (
        t: t,
        stats: measurablesStateStatsQuery,
        query: MeasurableQueryIndex.query,
      ) =>
    <Div>
      <Div float=`left>
        {switch (stats) {
         | Success(Some(r)) =>
           <Div
             float=`left
             styles=[
               Css.style([
                 FC.PageCard.HeaderRow.Styles.itemTopPadding,
                 FC.PageCard.HeaderRow.Styles.itemBottomPadding,
               ]),
             ]>
             {stateLink(
                [|`OPEN|],
                "Open",
                r.openTotal,
                query.state == Some([|`OPEN|]),
              )}
             {stateLink(
                [|`JUDGEMENT_PENDING|],
                "Pending Resolution",
                r.pendingTotal,
                query.state == Some([|`JUDGEMENT_PENDING|]),
              )}
             {stateLink(
                [|`JUDGED, `CLOSED_AS_UNRESOLVED|],
                "Closed",
                r.closedTotal,
                query.state == Some([|`JUDGED, `CLOSED_AS_UNRESOLVED|]),
              )}
           </Div>
         | _ => "Error" |> ste
         }}
      </Div>
      <Div
        float=`right
        styles=[
          Css.style([
            FC.PageCard.HeaderRow.Styles.itemTopPadding,
            FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {Reducer.Components.paginationPage(t.reducerParams)}
      </Div>
    </Div>;

  let body = (t: t) => {
    let measurables =
      (
        switch (t.reducerParams.response) {
        | Success(r) => Some(r.edges)
        | _ => None
        }
      )
      |> E.O.toExn("");
    <MeasurableIndexTable
      measurables
      showExtraData=true
      channelId={Some(t.channel.id)}
      onSelect={e => Reducer.Components.sendSelectItem(t.reducerParams, e.id)}
    />;
  };
};

let load3Queries = (channelId, states, itemsPerPage, fn) =>
  (
    (reducer, channel, series, stats) =>
      (reducer, channel, series, stats) |> fn
  )
  |> E.F.flatten4Callbacks(
       MeasurableIndex__Logic.Reducer.make(
         ~itemsPerPage,
         ~callFnParams={channelId, states},
         ~subComponent=_,
       ),
       ChannelGet.component2(~id=channelId),
       SeriesCollectionGet.component2(~channelId),
       MeasurablesStateStatsGet.component2(~channelId),
     );

let make =
    (
      ~channelId: string,
      ~searchParams: MeasurableQueryIndex.query,
      ~loggedInUser: Types.user,
      ~itemsPerPage: int=20,
    ) => {
  let loadData = load3Queries(channelId, searchParams.state, itemsPerPage);

  loadData(
    ((reducerParams, channelQuery, seriesQuery, measurablesStateStatsQuery)) =>
    switch (
      MeasurableIndex__Logic.MeasurableIndexDataState.make({
        reducerParams,
        loggedInUser,
        channelQuery,
        seriesQuery,
      })
    ) {
    | InvalidIndexError(_) =>
      SLayout.LayoutConfig.make(
        ~head=E.React.null,
        ~body="Item Not Valid" |> Utils.ste,
      )
      |> SLayout.FullPage.makeWithEl
    | WithChannelButNotQuery(_c) =>
      SLayout.LayoutConfig.make(~head=E.React.null, ~body=<SLayout.Spin />)
      |> SLayout.FullPage.makeWithEl
    | LoadedAndUnselected(l) =>
      SLayout.LayoutConfig.make(
        ~head=
          LoadedAndUnselected.header(
            l,
            measurablesStateStatsQuery,
            searchParams,
          ),
        ~body=LoadedAndUnselected.body(l),
      )
      |> SLayout.FullPage.makeWithEl
    | LoadedAndSelected(l) =>
      <>
        {SLayout.LayoutConfig.make(
           ~head=LoadedAndSelected.header(l, reducerParams.send),
           ~body=LoadedAndSelected.body(l),
         )
         |> SLayout.FullPage.makeWithEl}
        {LoadedAndSelected.body2(l)}
      </>

    | WithoutChannel(_) =>
      SLayout.LayoutConfig.make(
        ~head=E.React.null,
        ~body="No channel." |> Utils.ste,
      )
      |> SLayout.FullPage.makeWithEl
    }
  );
};