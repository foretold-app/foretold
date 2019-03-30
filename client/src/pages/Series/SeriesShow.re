open Utils;
open Style.Grid;
open Foretold__GraphQL;
open Context.Primary;

module GetMeasurablesReducerConfig = {
  type itemType = Context.Primary.Measurable.t;
  type callFnParams = string;
  let getId = (e: Context.Primary.Measurable.t) => e.id;
  let callFn = (e: callFnParams) =>
    Foretold__GraphQL.Queries.Measurables.component3(~seriesId=e);
  let isEqual = (a: itemType, b: itemType) => a.id == b.id;
};

module SelectWithPaginationReducer =
  SelectWithPaginationReducerFunctor.Make(GetMeasurablesReducerConfig);

module Styles = {
  open Css;
  let header =
    style([
      backgroundColor(hex("f5f7f9")),
      padding2(~v=`px(10), ~h=`px(13)),
      border(`px(1), `solid, `hex("e8f2f9")),
      borderRadius(`px(3)),
      marginBottom(`px(10)),
    ]);

  let topPart =
    style([
      maxHeight(`px(300)),
      overflowY(`auto),
      borderRadius(`px(2)),
      border(`px(1), `solid, `hex("ddd")),
      marginTop(`em(2.)),
      marginBottom(`em(2.)),
    ]);
};

let load2Queries = (channelId, seriesId, itemsPerPage, fn) =>
  ((a, b, c) => (a, b, c) |> fn)
  |> E.F.flatten3Callbacks(
       SelectWithPaginationReducer.make(
         ~itemsPerPage,
         ~callFnParams=seriesId,
         ~subComponent=_,
       ),
       Queries.Channel.component2(~id=channelId),
       Queries.Series.component(~id=seriesId),
     );

let component = ReasonReact.statelessComponent("Measurables");
let make =
    (
      ~channelId: string,
      ~id: string,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let loadData = load2Queries(channelId, id, 50);
    let lmake = SLayout.LayoutConfig.make;

    loadData(((selectWithPaginationParams, channel, series)) =>
      lmake(
        ~head=
          switch (channel, series, selectWithPaginationParams.selection) {
          | (
              Success(channel),
              Some((series: Context.Primary.Series.t)),
              Some(selection),
            ) =>
            <>
              {SLayout.seriesHead(channel, series.name |> E.O.default(""))}
              {
                SelectWithPaginationReducer.Components.deselectButton(
                  selectWithPaginationParams.send,
                )
              }
              {
                SelectWithPaginationReducer.Components.correctButtonDuo(
                  selectWithPaginationParams,
                )
              }
            </>
          | (
              Success(channel),
              Some((series: Context.Primary.Series.t)),
              None,
            ) =>
            <>
              {SLayout.seriesHead(channel, series.name |> E.O.default(""))}
              {
                SelectWithPaginationReducer.Components.correctButtonDuo(
                  selectWithPaginationParams,
                )
              }
            </>
          | _ => <div />
          },
        ~body=
          switch (
            selectWithPaginationParams.response,
            selectWithPaginationParams.selection,
          ) {
          | (_, Some(measurable)) =>
            <C.Measurable.FullPresentation id={measurable.id} loggedInUser />
          | (Success(measurables), None) =>
            <C.Measurables.SeriesTable
              measurables
              selected=None
              onClick=(
                id =>
                  SelectWithPaginationReducer.Components.sendSelectItem(
                    selectWithPaginationParams,
                    id,
                  )
              )
            />
          | _ => <div />
          },
      )
      |> layout
    )
    |> E.React.makeToEl;
  },
};