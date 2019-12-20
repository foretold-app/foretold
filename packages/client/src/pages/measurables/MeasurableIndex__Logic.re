module ReducerConfig = {
  type itemType = Types.measurable;

  type callFnParams = {
    channelId: string,
    states: option(array(Types.measurableState)),
  };

  let getId = (params: itemType) => params.id;

  let callFn = (params: callFnParams) =>
    MeasurablesGet.component(
      ~channelId=Some(params.channelId),
      ~states=Some(params.states |> E.O.default([||])),
      (),
    );

  let isEqual = (a: itemType, b: itemType) => {
    let result =
      switch (a.updatedAt, b.updatedAt) {
      | (Some(a), Some(b)) => MomentRe.Moment.isSame(a, b)
      | _ => false
      };
    result;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

type channel = Types.channel;
type seriesCollection = array(SeriesCollectionGet.series);
type reducerParams = Reducer.Types.reducerParams;
type seriesQuery = HttpResponse.t(seriesCollection);
type channelQuery = HttpResponse.t(channel);
type measurablesStateStatsQuery =
  HttpResponse.t(option(MeasurablesStateStatsGet.stats));

type loadedAndSelected = {
  reducerParams,
  channel,
  seriesCollection,
  itemState: Reducer.Types.itemSelected,
  selectedMeasurable: ReducerConfig.itemType,
};

type loadedAndUnselected = {
  reducerParams,
  channel,
  seriesCollection,
};

type withChannelButNotQuery = {
  reducerParams,
  channel,
  seriesQuery,
};

type state =
  | WithoutChannel(channelQuery)
  | InvalidIndexError(channel)
  | WithChannelButNotQuery(withChannelButNotQuery)
  | LoadedAndUnselected(loadedAndUnselected)
  | LoadedAndSelected(loadedAndSelected);

type input = {
  reducerParams,
  channelQuery,
  seriesQuery,
};


// @todo: To make a component.
let make = (input: input) =>
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
      ItemUnselected(_),
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