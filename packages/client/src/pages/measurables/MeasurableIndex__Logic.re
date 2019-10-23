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
      ~states=params.states |> E.O.default([||]) |> Array.map(r => Some(r)),
      (),
    );

  let isEqual = (a: itemType, b: itemType) => a.id == b.id;
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

type channel = Types.channel;
type seriesCollection = array(SeriesCollectionGet.series);
type reducerParams = Reducer.Types.reducerParams;
type seriesQuery = HttpResponse.t(seriesCollection);
type channelQuery = HttpResponse.t(channel);
type measurablesStateStatsQuery =
  HttpResponse.t(option(MeasurablesStateStatsGet.stats));

module LoadedAndSelected = {
  type t = {
    reducerParams,
    channel,
    seriesCollection,
    itemState: Reducer.Types.itemSelected,
    selectedMeasurable: ReducerConfig.itemType,
  };
};

module LoadedAndUnselected = {
  type t = {
    reducerParams,
    channel,
    seriesCollection,
  };
};

module WithChannelButNotQuery = {
  type t = {
    reducerParams,
    channel,
    seriesQuery,
  };
};

type state =
  | WithoutChannel(channelQuery)
  | InvalidIndexError(channel)
  | WithChannelButNotQuery(WithChannelButNotQuery.t)
  | LoadedAndUnselected(LoadedAndUnselected.t)
  | LoadedAndSelected(LoadedAndSelected.t);

type input = {
  reducerParams,
  channelQuery,
  seriesQuery,
};

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
