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

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

type channel = Types.channel;
type seriesCollection = array(SeriesCollectionGet.series);
type loggedInUser = Types.user;
type reducerParams = Reducer.Types.reducerParams;
type seriesQuery = HttpResponse.t(seriesCollection);
type channelQuery = HttpResponse.t(channel);
type measurablesStateStatsQuery =
  HttpResponse.t(option(MeasurablesStateStatsGet.stats));

module LoadedAndSelected = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    seriesCollection,
    itemState: Reducer.Types.itemSelected,
    selectedMeasurable: ReducerConfig.itemType,
  };
};

module LoadedAndUnselected = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    seriesCollection,
  };
};

module WithChannelButNotQuery = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    seriesQuery,
  };
};

module MeasurableIndexDataState = {
  type state =
    | WithoutChannel(channelQuery)
    | InvalidIndexError(channel)
    | WithChannelButNotQuery(WithChannelButNotQuery.t)
    | LoadedAndUnselected(LoadedAndUnselected.t)
    | LoadedAndSelected(LoadedAndSelected.t);

  type input = {
    reducerParams,
    loggedInUser,
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
          loggedInUser: input.loggedInUser,
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
        loggedInUser: input.loggedInUser,
        seriesCollection,
      })
    | (_, Success(channel), _, _) =>
      WithChannelButNotQuery({
        channel,
        reducerParams: input.reducerParams,
        loggedInUser: input.loggedInUser,
        seriesQuery: input.seriesQuery,
      })
    | _ => WithoutChannel(input.channelQuery)
    };
};