open Utils;
open Style.Grid;
open MeasurableIndex__Logic;

module LoadedAndSelected = {
  let header = (t: loadedAndSelected, send: Reducer.Types.send) =>
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

  let measurablePage = (t: loadedAndSelected) =>
    <MeasurablePage
      id={t.selectedMeasurable.id}
      key={t.selectedMeasurable.id}
    />;

  let measurableBottomSection = (t: loadedAndSelected) =>
    <MeasurableBottomSection measurable={t.selectedMeasurable} />;
};

module LoadedAndUnselected = {
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
        t: loadedAndUnselected,
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

  let measurableIndexTable = (t: loadedAndUnselected) => {
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
      onSelect={(e: Types.measurable) =>
        Reducer.Components.sendSelectItem(t.reducerParams, e.id)
      }
    />;
  };
};

let toLayoutInput =
    (
      send: Reducer.Types.send,
      selectedState: MeasurableQueryIndex.query,
      stats: measurablesStateStatsQuery,
      state: state,
    ) => {
  switch (state) {
  | LoadedAndUnselected(l) =>
    <SLayout
      head={LoadedAndUnselected.header(l, stats, selectedState)} isFluid=true>
      {LoadedAndUnselected.measurableIndexTable(l)}
    </SLayout>

  | LoadedAndSelected(l) =>
    <>
      {<SLayout head={LoadedAndSelected.header(l, send)} isFluid=true>
         {LoadedAndSelected.measurablePage(l)}
       </SLayout>}
      {LoadedAndSelected.measurableBottomSection(l)}
    </>

  | WithoutChannel(_) =>
    <SLayout head=E.React2.null isFluid=true> {"No channel." |> ste} </SLayout>

  | InvalidIndexError(_) =>
    <SLayout head=E.React2.null isFluid=true>
      {"Item Not Valid" |> ste}
    </SLayout>

  | WithChannelButNotQuery(_c) =>
    <SLayout head=E.React2.null isFluid=true> <Spin /> </SLayout>
  };
};