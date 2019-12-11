module Config = {
  type itemType = Types.measurable;
  type callFnParams = string;

  let getId = (e: Types.measurable) => e.id;

  let callFn = (seriesId: callFnParams) =>
    MeasurablesGet.component(
      ~seriesId=Some(seriesId),
      ~states=Some([|`OPEN|]),
      (),
    );

  let isEqual = (a: itemType, b: itemType) => a.id == b.id;
};

module Reducer = PaginationFunctor.Make(Config);

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
       subComponent =>
         <Reducer itemsPerPage callFnParams=seriesId subComponent />,
       ChannelGet.component2(~id=channelId),
       SeriesGet.component(~id=seriesId),
     );

[@react.component]
let make = (~channelId: string, ~id: string) => {
  let loadData = load2Queries(channelId, id, 50);

  loadData(((selectWithPaginationParams, channel, series)) =>
    <SLayout
      head={
        switch (channel, series, selectWithPaginationParams.selection) {
        | (
            Success(channel),
            Some((series: Types.series)),
            Some(_selection),
          ) =>
          <>
            {SLayout.seriesHead(channel, series.name |> E.O.default(""))}
            {Reducer.Components.deselectButton(
               selectWithPaginationParams.send,
             )}
            {Reducer.Components.correctButtonDuo(selectWithPaginationParams)}
          </>
        | (Success(channel), Some((series: Types.series)), None) =>
          <>
            {SLayout.seriesHead(channel, series.name |> E.O.default(""))}
            {Reducer.Components.correctButtonDuo(selectWithPaginationParams)}
          </>
        | _ => <div />
        }
      }>
      {switch (
         selectWithPaginationParams.response,
         selectWithPaginationParams.selection,
       ) {
       | (_, Some(measurable)) => <Measurable id={measurable.id} />
       | (Success(connection), None) =>
         <MeasurablesSeriesTable
           measurables={connection.edges}
           selected=None
           onClick={id =>
             Reducer.Components.sendSelectItem(selectWithPaginationParams, id)
           }
         />
       | _ => <div />
       }}
    </SLayout>
  );
};