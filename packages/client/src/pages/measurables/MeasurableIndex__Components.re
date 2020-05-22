open Utils;
open Style.Grid;
open MeasurableIndex__Logic;

module LoadedAndSelected = {
  let header = (subState: loadedAndSelected, send: Reducer.send) =>
    <>
      <Div
        float=`left
        styles=[
          Css.style([
            ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
            ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {Reducer.Components.deselectButton(send)}
      </Div>
      <Div
        float=`right
        styles=[
          Css.style([
            ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
            ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {Reducer.Components.paginationItem(subState.reducerParams)}
      </Div>
    </>;
};

module LoadedAndUnselected = {
  // @todo: To make a component.
  let stateLink = (state, text, num: int, isActive) =>
    <ForetoldComponents.Tab2
      isActive
      number=num
      onClick={LinkType.onClick(
        Relative(
          MeasurableQuery.make(Some(state)) |> MeasurableQuery.toUrlParams,
        ),
      )}>
      {text |> ste}
    </ForetoldComponents.Tab2>;

  // @todo: To make a component.
  let header =
      (
        subState: loadedAndUnselected,
        stats: measurablesStateStatsQuery,
        query: MeasurableQuery.query,
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
                 ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
                 ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
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
            ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
            ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {<ForetoldComponents.Button
           variant=ForetoldComponents.Button.Primary
           isDisabled=false
           size=ForetoldComponents.Button.Small
           className=Css.(style([marginRight(`em(1.5))]))
           onClick={e =>
             LinkType.onClick(Internal(MeasurableNew(channelId)), e)
           }>
           {"New Question" |> Utils.ste}
         </ForetoldComponents.Button>
         |> E.React2.showIf(channelId != "")}
        {Reducer.Components.paginationPage(subState.reducerParams)}
      </Div>
    </Div>;

  // @todo: To make a component.
  let measurableIndexTable = (subState: loadedAndUnselected) => {
    let measurables =
      (
        switch (subState.reducerParams.response) {
        | Success(r) => Some(r.edges)
        | _ => None
        }
      )
      |> E.O.toExn("");
    <MeasurableIndexTable
      measurables
      showExtraData=true
      channelId={Some(subState.channel.id)}
      onSelect={(e: Types.measurable) =>
        Reducer.Components.sendSelectItem(subState.reducerParams, e.id)
      }
    />;
  };
};

// @todo: To make a component.
let toLayoutInput =
    (
      send: Reducer.send,
      selectedState: MeasurableQuery.query,
      stats: measurablesStateStatsQuery,
      state: state,
      channelId: string,
    ) => {
  switch (state) {
  | LoadedAndUnselected(subState) =>
    <SLayout
      head={LoadedAndUnselected.header(
        subState,
        stats,
        selectedState,
        channelId,
      )}
      container=`fluid>
      {LoadedAndUnselected.measurableIndexTable(subState)}
    </SLayout>

  | LoadedAndSelected(subState) =>
    <>
      <SLayout
        head={LoadedAndSelected.header(subState, send)} container=`fluid>
        <MeasurablePage
          measurable={subState.selectedMeasurable}
          key={subState.selectedMeasurable.id}
        />
      </SLayout>
      <MeasurableBottomSection
        measurable={subState.selectedMeasurable}
        key={"measurable-bottom-section" ++ subState.selectedMeasurable.id}
      />
    </>

  | WithoutChannel(_) => <NotFoundPage />

  | InvalidIndexError(_) =>
    <SLayout container=`fluid> {"Item Not Valid" |> ste} </SLayout>

  | WithChannelButNotQuery(_c) =>
    <SLayout container=`fluid> <Spin /> </SLayout>
  };
};