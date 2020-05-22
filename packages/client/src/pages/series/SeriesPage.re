open Style.Grid;

module Config = {
  type itemType = Types.measurable;
  type callFnParams = string;

  let getId = (e: Types.measurable) => e.id;

  let onItemDeselected = _ => ();
  let onItemSelected = (_: option(itemType)) => ();

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

// @todo: To make a component.
let load2Queries = (channelId, seriesId, itemsPerPage, fn) =>
  ((a, b, c) => (a, b, c) |> fn)
  |> E.F.flatten3Callbacks(
       subComponent =>
         <Reducer itemsPerPage callFnParams=seriesId subComponent />,
       ChannelGet.component2(~id=channelId),
       SeriesGet.component(~id=seriesId),
     );

[@react.component]
let make = (~channelId: string, ~seriesId: string) => {
  let pagination =
      (reducerParams: Reducer.state, series: option(Types.series)) => {
    switch (series) {
    | Some(series) =>
      <Div>
        <Div
          float=`left
          styles=[
            Css.style([
              ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
              ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
            ]),
          ]>
          <SLayout.SeriesHead seriesName={series.name |> E.O.default("")} />
        </Div>
        <Div
          float=`right
          styles=[
            Css.style([
              ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
              ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
            ]),
          ]>
          <ForetoldComponents.Base.Button
            variant=ForetoldComponents.Base.Button.Primary
            size=ForetoldComponents.Base.Button.Small
            className=Css.(style([marginRight(`em(1.5))]))
            onClick={e =>
              LinkType.onClick(
                Internal(SeriesEdit(series.channelId, series.id)),
                e,
              )
            }>
            {"Edit Series" |> Utils.ste}
          </ForetoldComponents.Base.Button>
          <Reducer.Components.PaginationPage state=reducerParams />
        </Div>
      </Div>
    | _ => <Null />
    };
  };

  load2Queries(channelId, seriesId, 50, ((reducerParams, channel, series)) => {
    <SLayout container=`fluid head={pagination(reducerParams, series)}>
      {switch (reducerParams.response, reducerParams.selection) {
       | (_, Some(measurable)) => <MeasurablePage measurable />

       | (Success(connection), None) =>
         <MeasurablesSeriesTable
           measurables={connection.edges}
           selected=None
           onClick={id =>
             Reducer.Components.sendSelectItem(reducerParams, id)
           }
         />

       | _ => <div />
       }}
    </SLayout>
  });
};