module GetMeasurablesReducerConfig = {
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

module SelectWithPaginationReducer =
  SelectWithPaginationReducerFunctor.Make(GetMeasurablesReducerConfig);

module Types = {
  module ChannelQuery = Context.Primary.Channel;
  type channel = ChannelQuery.t;

  module SeriesCollectionQuery = Foretold__GraphQL.Queries.SeriesCollection;
  type seriesCollectionType = Js.Array.t(SeriesCollectionQuery.series);

  type loggedInUser = Context.Primary.User.t;

  type query = E.HttpResponse.t(seriesCollectionType);

  type page = int;
  module ReducerTypes = SelectWithPaginationReducer.Types;
  type reducerParams = ReducerTypes.reducerParams;
  /* module ReducerItemState = SelectWithPaginationReducer.Reducers.ItemState; */
  module ReducerParams = SelectWithPaginationReducer.Reducers.ReducerParams;
};
open Types;

module LoadedAndSelected = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    seriesCollection: seriesCollectionType,
    itemState: ReducerTypes.itemSelected,
    selectedMeasurable: GetMeasurablesReducerConfig.itemType,
  };
};

module LoadedAndUnselected = {
  type t = {
    reducerParams,
    loggedInUser,
    channel,
    seriesCollection: seriesCollectionType,
  };

  let pageNumber = (t: t) => 3;

  let filteredSeriesCollection = (t: t) =>
    t.seriesCollection
    |> E.A.filter((x: SeriesCollectionQuery.series) =>
         x.measurableCount !== Some(0)
       );

  let shouldShowSeriesCollection = (t: t) =>
    pageNumber(t) == 0 && filteredSeriesCollection(t) |> E.A.length > 0;
  //  let findMeasurableIndexOfMeasurableId = (t: t, id) =>
  //    Context.Primary.Measurable.(
  //      switch (t.reducerParams.response) {
  //      | Success(m) => m.edges |> E.A.findIndex(r => r.id == id)
  //      | _ => None
  //      }
  //    );
  //
  //  let selectMeasurableOfMeasurableId = (t: t, id) =>
  //    findMeasurableIndexOfMeasurableId(t, id)
  //    |> E.O.fmap(e => ReducerTypes.SelectIndex(e));
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
        ItemSelected({selectedIndex}),
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