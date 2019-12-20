open Style.Grid;

module Styles = {
  open Css;
  let header =
    style(
      [padding2(~v=`em(1.5), ~h=`em(1.5))]
      @ FC.Base.BaseStyles.fullWidthFloatLeft,
    );
  let link = style([marginTop(em(1.))]);
};

[@react.component]
let make = (~measurable) => {
  <>
    <Div styles=[Styles.header]>
      <Div flexDirection=`row>
        <Div flex={`num(3.)}>
          <FC.PageCard.H1>
            <MeasurableItems.LinkMeasurable m=measurable />
          </FC.PageCard.H1>
          <StatusDisplay measurable />
        </Div>
        <Div flex={`num(1.)}>
          <MeasurableItems.Series m=measurable />
          <MeasurableItems.CreatorLink m=measurable />
          <MeasurableItems.ResolutionEndpoint m=measurable />
          <MeasurableItems.EndpointResponse m=measurable />
          <MeasurableItems.QuestionLink m=measurable />
        </Div>
      </Div>
      <MeasurableItems.Description m=measurable />
    </Div>
    <MeasurementForm
      measurable
      measurableId={measurable.id}
      isCreator={measurable.iAmOwner == Some(true)}
    />
  </>;
};