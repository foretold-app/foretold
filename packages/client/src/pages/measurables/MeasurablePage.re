open Style.Grid;

module Styles = {
  open Css;
  let header =
    style(
      [padding2(~v=`em(1.5), ~h=`em(1.5))]
      @ FC.Base.BaseStyles.fullWidthFloatLeft,
    );
  let link = style([marginTop(em(1.))]);
  let description = style([paddingTop(`em(1.5))]);
};

[@react.component]
let make = (~measurable) => {
  <>
    <Div styles=[Styles.header]>
      <Div flexDirection=`row>
        <Div flex={`num(3.)}>
          <FC.PageCard.H1>
            {MeasurableItems.link(~m=measurable)}
          </FC.PageCard.H1>
          <StatusDisplay measurable />
        </Div>
        <Div flex={`num(1.)}>
          {MeasurableItems.series(~m=measurable) |> E.O.React.defaultNull}
          {MeasurableItems.creatorLink(~m=measurable) |> E.O.React.defaultNull}
          {MeasurableItems.resolutionEndpoint(~m=measurable)
           |> E.O.React.defaultNull}
          {MeasurableItems.endpointResponse(~m=measurable)
           |> E.O.React.defaultNull}
          {MeasurableItems.questionLink(~m=measurable)}
        </Div>
      </Div>
      {MeasurableItems.description(~m=measurable)
       |> E.O.React.fmapOrNull(d =>
            <Div styles=[Styles.description]> d </Div>
          )}
    </Div>
    <MeasurementForm
      measurable
      measurableId={measurable.id}
      isCreator={measurable.iAmOwner == Some(true)}
    />
  </>;
};