open Rationale.Function.Infix;
open Utils;

let itemsPerPage = 20;
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
module ItemState = SelectWithPaginationReducer.Reducers.ItemState;

type loadedResources = {
  seriesCollection: seriesCollectionType,
  measurables: measurablesType,
};

let toResources = (r: (seriesCollectionType, measurablesType)) => {
  let (seriesCollection, measurables) = r;
  {seriesCollection, measurables};
};

module LoadedAndSelected = {
  type t = {
    reducerParams,
    selectedMeasurable: MeasurablesQuery.t,
    itemState: ReducerTypes.itemSelected,
    channel,
    loggedInUser,
    loadedResources,
  };

  module Actions = {
    let itemAtIndexExists = (t: t, index) => {
      let atLeast0 = i => i >= 0;
      let exists = E.A.get(t.loadedResources.measurables) ||> E.O.isSome;
      atLeast0(index) && exists(index);
    };
    let canIncrement = (t: t) =>
      itemAtIndexExists(t, t.itemState.selectedIndex + 1);
    let canDecrement = (t: t) =>
      itemAtIndexExists(t, t.itemState.selectedIndex - 1);
  };
};

module LoadedAndUnselected = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    loadedResources,
  };

  module Actions = {
    let canIncrement = (t: t) =>
      E.A.length(t.loadedResources.measurables) == itemsPerPage;
    let canDecrement = (t: t) =>
      t.reducerParams.itemState
      |> SelectWithPaginationReducer.Reducers.ItemState.pageNumber > 0;
  };

  let filteredSeriesCollection = (t: t) =>
    t.loadedResources.seriesCollection
    |> E.A.filter((x: SeriesCollectionQuery.series) =>
         x.measurableCount !== Some(0)
       );

  let shouldShowSeriesCollection = (t: t) =>
    t.reducerParams.itemState
    |> ItemState.pageNumber == 0
    && filteredSeriesCollection(t)
    |> E.A.length > 0;

  let findMeasurableIndexOfMeasurableId = (t: t, id) =>
    t.loadedResources.measurables
    |> E.A.findIndex((r: MeasurablesQuery.t) => r.id == id);

  let selectMeasurableOfMeasurableId = (t: t, id) => {
    Js.log2(id, findMeasurableIndexOfMeasurableId(t, id));
    findMeasurableIndexOfMeasurableId(t, id)
    |> E.O.fmap(e => ReducerTypes.SelectIndex(e));
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
  type t = E.HttpResponse.t(ChannelQuery.t);
};

module MeasurableIndexDataState = {
  type state =
    | WithoutChannel(WithoutChannel.t)
    | InvalidIndexError(ChannelQuery.t)
    | WithChannelButNotQuery(WithChannelButNotQuery.t)
    | LoadedAndUnselected(LoadedAndUnselected.t)
    | LoadedAndSelected(LoadedAndSelected.t);

  let findMeasurable =
      (index: int, query: (seriesCollectionType, measurablesType)) => {
    let (_, measurables) = query;
    E.A.get(measurables, index);
  };

  type input = {
    reducerParams: ReducerTypes.reducerParams,
    loggedInUser,
    channel: E.HttpResponse.t(ChannelQuery.t),
    query,
  };

  let make = (u: input) =>
    switch (u.channel, u.query, u.reducerParams.itemState) {
    | (
        Success(channel),
        Success(r),
        ItemSelected({pageNumber, selectedIndex}),
      ) =>
      switch (findMeasurable(selectedIndex, r)) {
      | Some(measurable) =>
        LoadedAndSelected({
          channel,
          itemState: {
            pageNumber,
            selectedIndex,
          },
          reducerParams: u.reducerParams,
          selectedMeasurable: measurable,
          loggedInUser: u.loggedInUser,
          loadedResources: r |> toResources,
        })
      | _ => InvalidIndexError(channel)
      }
    | (Success(channel), Success(r), ItemUnselected(_)) =>
      LoadedAndUnselected({
        channel,
        reducerParams: u.reducerParams,
        loggedInUser: u.loggedInUser,
        loadedResources: r |> toResources,
      })
    | (Success(channel), Error(_) | Loading, _) =>
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