module ReducerConfig = {
  type itemType = Context.Primary.Measurable.t;
  type callFnParams = {
    channelId: string,
    states: array(Context.Primary.MeasurableState.t),
  };

  let getId = (params: Context.Primary.Measurable.t) => params.id;
  let callFn = (params: callFnParams) =>
    Foretold__GraphQL.Queries.Measurables.component2(
      ~channelId=params.channelId,
      ~states=params.states |> Array.map(r => Some(r)),
    );
  let isEqual = (a: itemType, b: itemType) => a.id == b.id;
};

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

module Types = {
  type channel = Context.Primary.Channel.t;

  type seriesCollection =
    array(Foretold__GraphQL.Queries.SeriesCollection.series);

  type loggedInUser = Context.Primary.User.t;

  type reducerParams = Reducer.Types.reducerParams;

  type seriesQuery = E.HttpResponse.t(seriesCollection);

  type channelQuery = E.HttpResponse.t(channel);
};

open Types;

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
    | InvalidIndexError(Context.Primary.Channel.t)
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
      | Some(measurable) =>
        LoadedAndSelected({
          channel,
          reducerParams: input.reducerParams,
          loggedInUser: input.loggedInUser,
          itemState: {
            selectedIndex: selectedIndex,
          },
          selectedMeasurable: measurable,
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