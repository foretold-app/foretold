open Utils;
open Style.Grid;

module ReducerConfig = {
  type itemType = Types.measurable;

  type callFnParams = {
    channelId: string,
    states: option(array(Types.measurableState)),
  };

  let getId = (itemType: itemType) => itemType.id;

  let onItemDeselected = (params: callFnParams) => {
    let st =
      params.states |> MeasurableQuery.make |> MeasurableQuery.toUrlParams;
    let channelLink = Routing.Url.toString(ChannelShow(params.channelId));
    History.append(channelLink ++ st);
  };

  let onItemSelected = (measurable: option(itemType)) => {
    switch (measurable) {
    | Some(measurable) =>
      let measurableLink =
        Routing.Url.toString(
          MeasurableShow(measurable.channelId, measurable.id),
        );
      History.append(measurableLink);
    | _ => ()
    };
    ();
  };

  let callFn = (params: callFnParams) =>
    MeasurablesGet.component(
      ~channelId=Some(params.channelId),
      ~states=Some(params.states |> E.O.default([||])),
      (),
    );

  let isEqual = (a: itemType, b: itemType) => {
    switch (a.updatedAt, b.updatedAt) {
    | (Some(a), Some(b)) => MomentRe.Moment.isSame(a, b)
    | _ => false
    };
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

type seriesCollection = array(SeriesCollectionGet.series);
type seriesQuery = HttpResponse.t(seriesCollection);
type channelQuery = HttpResponse.t(Types.channel);
type measurablesStateStatsQuery =
  HttpResponse.t(option(MeasurablesStateStatsGet.stats));

type loadedAndSelected = {
  reducerParams: Reducer.state,
  channel: Types.channel,
  seriesCollection,
  itemState: int,
  selectedMeasurable: ReducerConfig.itemType,
};

type loadedAndUnselected = {
  reducerParams: Reducer.state,
  channel: Types.channel,
  seriesCollection,
};

type withChannelButNotQuery = {
  reducerParams: Reducer.state,
  channel: Types.channel,
  seriesQuery,
};

type state =
  | WithoutChannel(channelQuery)
  | InvalidIndexError(Types.channel)
  | WithChannelButNotQuery(withChannelButNotQuery)
  | LoadedAndUnselected(loadedAndUnselected)
  | LoadedAndSelected(loadedAndSelected);

let getReducedStateInOneSimpleForm =
    (
      reducerParams: Reducer.state,
      channelQuery: channelQuery,
      seriesQuery: seriesQuery,
    ) =>
  switch (
    reducerParams.item,
    channelQuery,
    seriesQuery,
    reducerParams.response,
  ) {
  | (
      ItemSelected(selectedIndex),
      Success(channel),
      Success(seriesCollection),
      Success(_),
    ) =>
    switch (reducerParams.selection) {
    | Some(selectedMeasurable) =>
      LoadedAndSelected({
        channel,
        reducerParams,
        itemState: selectedIndex,
        selectedMeasurable,
        seriesCollection,
      })
    | _ => InvalidIndexError(channel)
    }

  | (
      ItemUnselected(_) | ItemDeselected(_),
      Success(channel),
      Success(seriesCollection),
      Success(_),
    ) =>
    LoadedAndUnselected({channel, reducerParams, seriesCollection})

  | (_, Success(channel), _, _) =>
    WithChannelButNotQuery({channel, reducerParams, seriesQuery})

  | _ => WithoutChannel(channelQuery)
  };

module MeasurableHeader = {
  [@react.component]
  let make = (~subState: loadedAndSelected, ~send: Reducer.send) =>
    <>
      <Div
        float=`left
        styles=[
          Css.style([
            ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
            ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        <Reducer.Components.DeselectButton send />
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

module StateLink = {
  [@react.component]
  let make = (~state, ~text, ~num: int, ~isActive) =>
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
};

module TableHeader = {
  [@react.component]
  let make =
      (
        ~subState: loadedAndUnselected,
        ~stats: measurablesStateStatsQuery,
        ~query: MeasurableQuery.query,
        ~channelId,
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
             <StateLink
               state=[|`OPEN|]
               text="Open"
               num={r.openTotal}
               isActive={query.state == Some([|`OPEN|])}
             />
             <StateLink
               state=[|`JUDGEMENT_PENDING|]
               text="Pending Resolution"
               num={r.pendingTotal}
               isActive={query.state == Some([|`JUDGEMENT_PENDING|])}
             />
             <StateLink
               state=[|`JUDGED, `CLOSED_AS_UNRESOLVED|]
               text="Closed"
               num={r.closedTotal}
               isActive={
                 query.state == Some([|`JUDGED, `CLOSED_AS_UNRESOLVED|])
               }
             />
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
};

module MeasurableTable = {
  [@react.component]
  let make = (~subState: loadedAndUnselected) => {
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

module LayoutInput = {
  [@react.component]
  let make =
      (
        ~send: Reducer.send,
        ~selectedState: MeasurableQuery.query,
        ~stats: measurablesStateStatsQuery,
        ~state: state,
        ~channelId: string,
      ) => {
    switch (state) {
    | LoadedAndUnselected(subState) =>
      <SLayout
        head={<TableHeader subState stats query=selectedState channelId />}
        container=`fluid>
        <MeasurableTable subState />
      </SLayout>

    | LoadedAndSelected(subState) =>
      <>
        <SLayout head={<MeasurableHeader subState send />} container=`fluid>
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
};

[@react.component]
let make = (~channelId: string, ~searchParams: MeasurableQuery.query) =>
  ChannelGet.component2(~id=channelId, channelQuery =>
    SeriesCollectionGet.component2(~channelId, seriesQuery =>
      MeasurablesStateStatsGet.component2(~channelId, statsQuery =>
        <Reducer
          callFnParams={channelId, states: searchParams.state}
          subComponent={(reducerParams: Reducer.state) =>
            <LayoutInput
              send={reducerParams.send}
              selectedState=searchParams
              stats=statsQuery
              state={getReducedStateInOneSimpleForm(
                reducerParams,
                channelQuery,
                seriesQuery,
              )}
              channelId
            />
          }
        />
      )
    )
  );