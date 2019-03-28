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

  type query = E.HttpResponse.t((seriesCollectionType, measurablesType));

  type page = int;
  module ReducerTypes = SelectWithPaginationReducer.Types;
  type reducerParams = ReducerTypes.reducerParams;
  module ReducerItemState = SelectWithPaginationReducer.Reducers.ItemState;
  module ReducerParams = SelectWithPaginationReducer.Reducers.ReducerParams;

  type loadedResources = {
    seriesCollection: seriesCollectionType,
    measurables: measurablesType,
  };
};
open Types;

let toResources = (r: (seriesCollectionType, measurablesType)) => {
  let (seriesCollection, measurables) = r;
  {seriesCollection, measurables};
};

module LoadedAndSelected = {
  type t = {
    reducerParams,
    loggedInUser,
    itemState: ReducerTypes.itemSelected,
    selectedMeasurable: MeasurablesQuery.t,
    channel,
    loadedResources,
  };
};

module LoadedAndUnselected = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    loadedResources,
  };

  let pageNumber = (t: t) => t.reducerParams |> ReducerParams.pageNumber;

  let filteredSeriesCollection = (t: t) =>
    t.loadedResources.seriesCollection
    |> E.A.filter((x: SeriesCollectionQuery.series) =>
         x.measurableCount !== Some(0)
       );

  let shouldShowSeriesCollection = (t: t) =>
    pageNumber(t) == 0 && filteredSeriesCollection(t) |> E.A.length > 0;

  let findMeasurableIndexOfMeasurableId = (t: t, id) =>
    t.loadedResources.measurables
    |> E.A.findIndex((r: MeasurablesQuery.t) => r.id == id);

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
    switch (u.reducerParams.itemState, u.channel, u.query) {
    | (
        ItemSelected({pageNumber, selectedIndex}),
        Success(channel),
        Success(resources),
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
          loadedResources: resources |> toResources,
        })
      | _ => InvalidIndexError(channel)
      }
    | (ItemUnselected(_), Success(channel), Success(r)) =>
      LoadedAndUnselected({
        channel,
        reducerParams: u.reducerParams,
        loggedInUser: u.loggedInUser,
        loadedResources: r |> toResources,
      })
    | (_, Success(channel), Error(_) | Loading) =>
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