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

module Redux = {
  type state = {
    page: int,
    selectedIndex: option(int),
  };

  let initialState = {page: 0, selectedIndex: None};

  type action =
    | NextPage
    | LastPage
    | Select(option(int))
    | SelectIncrement
    | SelectDecrement;
};

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
    page,
    pageSelectedIndex: int,
    selectedMeasurable: MeasurablesQuery.t,
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
      itemAtIndexExists(t, t.pageSelectedIndex + 1);
    let canDecrement = (t: t) =>
      itemAtIndexExists(t, t.pageSelectedIndex - 1);
    let selectDecrement = Redux.SelectDecrement;
    let selectIncrement = Redux.SelectIncrement;
    let deselect = Redux.Select(None);
  };
};

module LoadedAndUnselected = {
  type t = {
    page,
    loggedInUser,
    channel,
    loadedResources,
  };

  module Actions = {
    let canIncrement = (t: t) =>
      E.A.length(t.loadedResources.measurables) == itemsPerPage;
    let canDecrement = (t: t) => t.page > 0;
    let selectNextPage = Redux.NextPage;
    let selectLastPage = Redux.LastPage;
    let select = e => Redux.Select(e);
  };

  let filteredSeriesCollection = (t: t) =>
    t.loadedResources.seriesCollection
    |> E.A.filter((x: SeriesCollectionQuery.series) =>
         x.measurableCount !== Some(0)
       );

  let shouldShowSeriesCollection = (t: t) =>
    t.page == 0 && filteredSeriesCollection(t) |> E.A.length > 0;

  let findMeasurableIndexOfMeasurableId = (t: t, id) =>
    t.loadedResources.measurables
    |> E.A.findIndex((r: MeasurablesQuery.t) => r.id == id);

  let selectMeasurableOfMeasurableId = (t: t, id) => {
    Js.log2(id, findMeasurableIndexOfMeasurableId(t, id));
    findMeasurableIndexOfMeasurableId(t, id) |> Actions.select;
  };
};

module WithChannelButNotQuery = {
  type t = {
    page,
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
    page: int,
    pageSelectedIndex: option(int),
    loggedInUser,
    channel: E.HttpResponse.t(ChannelQuery.t),
    query,
  };

  let make = (u: input) =>
    switch (u.channel, u.pageSelectedIndex, u.query) {
    | (Success(channel), Some(index), Success(r)) =>
      switch (findMeasurable(index, r)) {
      | Some(measurable) =>
        LoadedAndSelected({
          channel,
          page: u.page,
          pageSelectedIndex: index,
          selectedMeasurable: measurable,
          loggedInUser: u.loggedInUser,
          loadedResources: r |> toResources,
        })
      | _ => InvalidIndexError(channel)
      }
    | (Success(channel), None, Success(r)) =>
      LoadedAndUnselected({
        channel,
        page: u.page,
        loggedInUser: u.loggedInUser,
        loadedResources: r |> toResources,
      })
    | (Success(channel), _, Error(_) | Loading) =>
      WithChannelButNotQuery({
        channel,
        page: u.page,
        loggedInUser: u.loggedInUser,
        query: u.query,
      })
    | _ => WithoutChannel(u.channel)
    };
};

module Components = {};