open Rationale.Function.Infix;
open Utils;

let itemsPerPage = 20;
module ChannelQuery = Context.Primary.Channel;
type channelType = ChannelQuery.t;

module SeriesCollectionQuery = Foretold__GraphQL.Queries.SeriesCollection;
type seriesCollectionType = Js.Array.t(SeriesCollectionQuery.series);

module MeasurablesQuery = Context.Primary.Measurable;
type measurablesType = array(MeasurablesQuery.t);

type loggedInUser = Context.Primary.User.t;

type query =
  E.HttpResponse.t((channelType, seriesCollectionType, measurablesType));

type page = int;

module Redux = {
  type state = {
    page: int,
    selectedIndex: option(int),
  };

  type action =
    | NextPage
    | LastPage
    | Select(option(int))
    | SelectIncrement
    | SelectDecrement;
};

type loadedResources = {
  channel: channelType,
  seriesCollection: seriesCollectionType,
  measurables: measurablesType,
};

let toResources = (r: (channelType, seriesCollectionType, measurablesType)) => {
  let (channel, seriesCollection, measurables) = r;
  {channel, seriesCollection, measurables};
};

module LoadedAndSelected = {
  type t = {
    page,
    pageSelectedIndex: int,
    selectedMeasurable: MeasurablesQuery.t,
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
    loadedResources,
  };

  module Actions = {
    let canIncrement = (t: t) =>
      E.A.length(t.loadedResources.measurables) < itemsPerPage;
    let canDecrement = (t: t) => t.page > 0;
    let selectNextPage = Redux.NextPage;
    let selectLastPage = Redux.LastPage;
  };

  let filteredSeriesCollection = (t: t) =>
    t.loadedResources.seriesCollection
    |> E.A.filter((x: SeriesCollectionQuery.series) =>
         x.measurableCount !== Some(0)
       );

  let shouldShowSeriesCollection = (t: t) =>
    t.page == 0 && filteredSeriesCollection(t) |> E.A.length > 0;
};

module Unloaded = {
  type t = {
    page,
    loggedInUser,
    query,
  };
};

module MeasurableIndexDataState = {
  type state =
    | InvalidIndexError
    | Unloaded(Unloaded.t)
    | LoadedAndUnselected(LoadedAndUnselected.t)
    | LoadedAndSelected(LoadedAndSelected.t);

  let findMeasurable =
      (
        index: int,
        query: (channelType, seriesCollectionType, measurablesType),
      ) => {
    let (_, _, measurables) = query;
    E.A.get(measurables, index);
  };

  type input = {
    page: int,
    pageSelectedIndex: option(int),
    loggedInUser,
    query,
  };

  let make = (u: input) =>
    switch (u.pageSelectedIndex, u.query) {
    | (Some(index), Success(r)) =>
      switch (findMeasurable(index, r)) {
      | Some(measurable) =>
        LoadedAndSelected({
          page: u.page,
          pageSelectedIndex: index,
          selectedMeasurable: measurable,
          loggedInUser: u.loggedInUser,
          loadedResources: r |> toResources,
        })
      | _ => InvalidIndexError
      }
    | (None, Success(r)) =>
      LoadedAndUnselected({
        page: u.page,
        loggedInUser: u.loggedInUser,
        loadedResources: r |> toResources,
      })
    | (_, Error(_) | Loading) =>
      Unloaded({page: u.page, loggedInUser: u.loggedInUser, query: u.query})
    };
};

module Components = {};