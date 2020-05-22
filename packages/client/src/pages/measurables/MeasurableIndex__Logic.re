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
type reducerParams = Reducer.reducerParams;
type seriesQuery = HttpResponse.t(seriesCollection);
type channelQuery = HttpResponse.t(Types.channel);
type measurablesStateStatsQuery =
  HttpResponse.t(option(MeasurablesStateStatsGet.stats));

type loadedAndSelected = {
  reducerParams,
  channel: Types.channel,
  seriesCollection,
  itemState: Reducer.itemSelected,
  selectedMeasurable: ReducerConfig.itemType,
};

type loadedAndUnselected = {
  reducerParams,
  channel: Types.channel,
  seriesCollection,
};

type withChannelButNotQuery = {
  reducerParams,
  channel: Types.channel,
  seriesQuery,
};

type state =
  | WithoutChannel(channelQuery)
  | InvalidIndexError(Types.channel)
  | WithChannelButNotQuery(withChannelButNotQuery)
  | LoadedAndUnselected(loadedAndUnselected)
  | LoadedAndSelected(loadedAndSelected);

type input = {
  reducerParams,
  channelQuery,
  seriesQuery,
};

let getState = (input: input): state =>
  switch (
    input.reducerParams.itemState,
    input.channelQuery,
    input.seriesQuery,
    input.reducerParams.response,
  ) {
  | (
      ItemSelected({selectedIndex}),
      Success(channel),
      Success(seriesCollection),
      Success(_),
    ) =>
    switch (input.reducerParams.selection) {
    | Some(selectedMeasurable) =>
      LoadedAndSelected({
        channel,
        reducerParams: input.reducerParams,
        itemState: {
          selectedIndex: selectedIndex,
        },
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
    LoadedAndUnselected({
      channel,
      reducerParams: input.reducerParams,
      seriesCollection,
    })

  | (_, Success(channel), _, _) =>
    WithChannelButNotQuery({
      channel,
      reducerParams: input.reducerParams,
      seriesQuery: input.seriesQuery,
    })

  | _ => WithoutChannel(input.channelQuery)
  };