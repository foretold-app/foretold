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
};

module LoadedAndUnselected = {
  // @todo: To make a component.
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

  // @todo: To make a component.
  let header =
      (
        t: loadedAndUnselected,
        stats: measurablesStateStatsQuery,
        query: MeasurableQueryIndex.query,
        channelId,
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
             <FC__Button
               variant=FC__Button.Primary
               isDisabled=false
               size=FC__Button.(Small)
               className=Css.(style([marginLeft(`em(1.5))]))
               onClick={e =>
                 LinkType.onClick(Internal(MeasurableNew(channelId)), e)
               }>
               {"New Question" |> Utils.ste}
             </FC__Button>
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

  // @todo: To make a component.
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

// @todo: To make a component.
let toLayoutInput =
    (
      send: Reducer.Types.send,
      selectedState: MeasurableQueryIndex.query,
      stats: measurablesStateStatsQuery,
      state: state,
      channelId: string,
    ) => {
  switch (state) {
  | LoadedAndUnselected(l) =>
    <SLayout
      head={LoadedAndUnselected.header(l, stats, selectedState, channelId)}
      container=`fluid>
      {LoadedAndUnselected.measurableIndexTable(l)}
    </SLayout>

  | LoadedAndSelected(l) =>
    <>
      <SLayout head={LoadedAndSelected.header(l, send)} container=`fluid>
        <MeasurablePage
          measurable={l.selectedMeasurable}
          key={l.selectedMeasurable.id}
        />
      </SLayout>
      <MeasurableBottomSection
        measurable={l.selectedMeasurable}
        key={"measurable-bottom-section" ++ l.selectedMeasurable.id}
      />
    </>

  | WithoutChannel(_) => <NotFoundPage />

  | InvalidIndexError(_) =>
    <SLayout container=`fluid> {"Item Not Valid" |> ste} </SLayout>

  | WithChannelButNotQuery(_c) =>
    <SLayout container=`fluid> <Spin /> </SLayout>
  };
};