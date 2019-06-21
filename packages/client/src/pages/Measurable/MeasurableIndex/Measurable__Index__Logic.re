module ReducerConfig = {
  type itemType = Context.Primary.Measurable.t;
  type callFnParams = {
    channelId: string,
    states: array(Context.Primary.MeasurableState.t),
  };
  let getId = (e: Context.Primary.Measurable.t) => e.id;
  let callFn = (e: callFnParams) =>
    Foretold__GraphQL.Queries.Measurables.component2(
      ~channelId=e.channelId,
      ~states=e.states |> E.A.fmap(r => Some(r)),
    );
  let isEqual = (a: itemType, b: itemType) => a.id == b.id;
};

module Reducer = SelectWithPaginationReducerFunctor.Make(ReducerConfig);

module Types = {
  type channel = Context.Primary.Channel.t;

  type seriesCollectionType =
    Js.Array.t(Foretold__GraphQL.Queries.SeriesCollection.series);

  type loggedInUser = Context.Primary.User.t;

  type query = E.HttpResponse.t(seriesCollectionType);

  type page = int;

  module ReducerTypes = Reducer.Types;

  type reducerParams = Reducer.Types.reducerParams;

  module ReducerParams = Reducer.Reducers.ReducerParams;
};

open Types;

module LoadedAndSelected = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    seriesCollection: seriesCollectionType,
    itemState: ReducerTypes.itemSelected,
    selectedMeasurable: ReducerConfig.itemType,
  };
};

module LoadedAndUnselected = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    seriesCollection: seriesCollectionType,
  };
};

module WithChannelButNotQuery = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    query,
  };
};

module WithoutChannel = {
  type t = E.HttpResponse.t(Context.Primary.Channel.t);
};

module MeasurableIndexDataState = {
  type state =
    | WithoutChannel(WithoutChannel.t)
    | InvalidIndexError(Context.Primary.Channel.t)
    | WithChannelButNotQuery(WithChannelButNotQuery.t)
    | LoadedAndUnselected(LoadedAndUnselected.t)
    | LoadedAndSelected(LoadedAndSelected.t);

  type input = {
    reducerParams: ReducerTypes.reducerParams,
    loggedInUser,
    channel: E.HttpResponse.t(Context.Primary.Channel.t),
    query,
  };

  let make = (input: input) =>
    switch (
      input.reducerParams.itemState,
      input.channel,
      input.query,
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
        query: input.query,
      })
    | _ => WithoutChannel(input.channel)
    };
};