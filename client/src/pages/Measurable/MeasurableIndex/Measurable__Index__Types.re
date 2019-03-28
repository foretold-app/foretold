open Rationale.Function.Infix;
open Utils;

module Types = {
  module ChannelQuery = Context.Primary.Channel;
  type channelType = ChannelQuery.t;
  type channel = ChannelQuery.t;

  module SeriesCollectionQuery = Foretold__GraphQL.Queries.SeriesCollection;
  type seriesCollectionType = Js.Array.t(SeriesCollectionQuery.series);

  module MeasurablesQuery = Context.Primary.Measurable;
  type measurablesType = array(MeasurablesQuery.t);

  type loggedInUser = Context.Primary.User.t;

  type query = E.HttpResponse.t(seriesCollectionType);

  type page = int;
  module ReducerTypes = SelectWithPaginationReducer.Types;
  type reducerParams = ReducerTypes.reducerParams;
  module ReducerItemState = SelectWithPaginationReducer.Reducers.ItemState;
  module ReducerParams = SelectWithPaginationReducer.Reducers.ReducerParams;
};
open Types;

module LoadedAndSelected = {
  type t = {
    reducerParams,
    loggedInUser,
    itemState: ReducerTypes.itemSelected,
    selectedMeasurable: MeasurablesQuery.t,
    channel,
    seriesCollection: seriesCollectionType,
  };
};

module LoadedAndUnselected = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    seriesCollection: seriesCollectionType,
  };

  let pageNumber = (t: t) => t.reducerParams |> ReducerParams.pageNumber;

  let filteredSeriesCollection = (t: t) =>
    t.seriesCollection
    |> E.A.filter((x: SeriesCollectionQuery.series) =>
         x.measurableCount !== Some(0)
       );

  let shouldShowSeriesCollection = (t: t) =>
    pageNumber(t) == 0 && filteredSeriesCollection(t) |> E.A.length > 0;

  let findMeasurableIndexOfMeasurableId = (t: t, id) =>
    switch (t.reducerParams.response) {
    | Success(m) => m |> E.A.findIndex((r: MeasurablesQuery.t) => r.id == id)
    | _ => None
    };

  let selectMeasurableOfMeasurableId = (t: t, id) =>
    findMeasurableIndexOfMeasurableId(t, id)
    |> E.O.fmap(e => ReducerTypes.SelectIndex(e));
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
  type t = E.HttpResponse.t(ChannelQuery.t);
};

module MeasurableIndexDataState = {
  type state =
    | WithoutChannel(WithoutChannel.t)
    | InvalidIndexError(ChannelQuery.t)
    | WithChannelButNotQuery(WithChannelButNotQuery.t)
    | LoadedAndUnselected(LoadedAndUnselected.t)
    | LoadedAndSelected(LoadedAndSelected.t);

  type input = {
    reducerParams: ReducerTypes.reducerParams,
    loggedInUser,
    channel: E.HttpResponse.t(ChannelQuery.t),
    query,
  };

  let make = (u: input) =>
    switch (
      u.reducerParams.itemState,
      u.channel,
      u.query,
      u.reducerParams.response,
    ) {
    | (
        ItemSelected({pageNumber, selectedIndex}),
        Success(channel),
        Success(seriesCollection),
        Success(_),
      ) =>
      switch (u.reducerParams.selection) {
      | Some(measurable) =>
        LoadedAndSelected({
          channel,
          reducerParams: u.reducerParams,
          loggedInUser: u.loggedInUser,
          itemState: {
            pageNumber,
            selectedIndex,
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
        reducerParams: u.reducerParams,
        loggedInUser: u.loggedInUser,
        seriesCollection,
      })
    | (_, Success(channel), _, _) =>
      WithChannelButNotQuery({
        channel,
        reducerParams: u.reducerParams,
        loggedInUser: u.loggedInUser,
        query: u.query,
      })
    | _ => WithoutChannel(u.channel)
    };
};

module Components = {};