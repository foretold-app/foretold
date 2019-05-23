open Utils;
open Style.Grid;
open Measurable__Index__Logic;

module ReducerParams = SelectWithPaginationReducer.Reducers.ReducerParams;

type measurablesStateStats =
  E.HttpResponse.t(
    option(Foretold__GraphQL.Queries.MeasurablesStateStats.stats),
  );

module LoadedAndSelected = {
  open Measurable__Index__Logic.LoadedAndSelected;

  let header = (t: t, send: SelectWithPaginationReducer.Types.send) =>
    <>
      <Div float=`left>
        {SelectWithPaginationReducer.Components.deselectButton(send)}
      </Div>
      <Div float=`right>
        <Div
          float=`left
          styles=[
            Css.style([
              Css.marginTop(`em(0.5)),
              Css.marginRight(`em(0.4)),
            ]),
          ]>
          {
            SelectWithPaginationReducer.Components.selectionOfX(
              t.reducerParams,
            )
            |> ste
          }
        </Div>
        {
          SelectWithPaginationReducer.Components.buttonDuo(
            Item,
            t.reducerParams,
          )
        }
      </Div>
    </>;

  let body = (t: t) =>
    <C.Measurable.FullPresentation
      id={t.selectedMeasurable.id}
      key={t.selectedMeasurable.id}
      loggedInUser={t.loggedInUser}
    />;
};

module LoadedAndUnselected = {
  open Measurable__Index__Logic.LoadedAndUnselected;
  module Styles = {
    open Css;
    let stateLink =
      [
        color(`hex("262626")),
        marginRight(`em(2.)),
        marginTop(`em(0.5)),
        float(`left),
        focus([textDecoration(`none)]),
        hover([color(`hex("262626"))]),
      ]
      |> style;
  };
  let stateLink = (state, text, num: int) =>
    <Foretold__Components__Link
      className=Styles.stateLink
      linkType={
        External(
          SearchResults.make(Some(state)) |> SearchResults.toUrlParams,
        )
      }>
      {text |> ste}
      {" - " ++ (num |> string_of_int) |> ste}
    </Foretold__Components__Link>;

  let header =
      (
        t: t,
        stats: measurablesStateStats,
        send: SelectWithPaginationReducer.Types.send,
      ) =>
    <Div>
      <Div float=`left>
        {
          switch (stats) {
          | Success(Some(r)) =>
            <>
              {stateLink(`OPEN, "Open", r.openTotal)}
              {
                stateLink(
                  `JUDGEMENT_PENDING,
                  "Pending Resolution",
                  r.pendingTotal,
                )
              }
              {stateLink(`JUDGED, "Closed", r.closedTotal)}
            </>
          | _ => <> </>
          }
        }
      </Div>
      <Div float=`right>
        <Div
          float=`left
          styles=[
            Css.style([
              Css.marginTop(`em(0.5)),
              Css.marginRight(`em(0.4)),
            ]),
          ]>
          {
            SelectWithPaginationReducer.Components.rangeOfX(t.reducerParams)
            |> ste
          }
        </Div>
        <Div float=`left>
          {
            SelectWithPaginationReducer.Components.buttonDuo(
              Page,
              t.reducerParams,
            )
          }
        </Div>
      </Div>
    </Div>;

  /* let seriesList = (t: t) =>
     <>
       {"Series List" |> ste |> E.React.inH2}
       {
         C.SeriesCollection.SeriesCards.make(
           t.channel.id,
           filteredSeriesCollection(t),
         )
       }
     </>; */

  let body = (t: t, send: SelectWithPaginationReducer.Types.send) => {
    let measurables =
      (
        switch (t.reducerParams.response) {
        | Success(r) => Some(r.edges)
        | _ => None
        }
      )
      |> E.O.toExn("");
    <>
      <C.Measurables.BasicTable
        measurables
        showExtraData=true
        channelId={Some(t.channel.id)}
        onSelect={
          e =>
            SelectWithPaginationReducer.Components.sendSelectItem(
              t.reducerParams,
              e.id,
            )
        }
      />
    </>;
  };
};

module MeasurableIndexDataState = {
  open Measurable__Index__Logic.MeasurableIndexDataState;

  let toLayoutInput =
      (
        send: SelectWithPaginationReducer.Types.send,
        stats: measurablesStateStats,
        state: state,
      ) => {
    let lmake = SLayout.LayoutConfig.make;
    switch (state) {
    | InvalidIndexError(channel) =>
      lmake(~head=E.React.null, ~body="Item Not Valid" |> ste)
    | WithChannelButNotQuery(c) =>
      lmake(~head=E.React.null, ~body="Loading Query..." |> ste)
    | LoadedAndUnselected(l) =>
      lmake(
        ~head=LoadedAndUnselected.header(l, stats, send),
        ~body=LoadedAndUnselected.body(l, send),
      )
    | LoadedAndSelected(l) =>
      lmake(
        ~head=LoadedAndSelected.header(l, send),
        ~body=LoadedAndSelected.body(l),
      )
    | WithoutChannel(channelResponse) =>
      lmake(~head=E.React.null, ~body="No channel." |> ste)
    };
  };
};