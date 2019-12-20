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
            <MeasurableItems.LinkMeasurable measurable />
          </FC.PageCard.H1>
          <StatusDisplay measurable />
        </Div>
        <Div flex={`num(1.)}>
          <MeasurableItems.Series measurable />
          <MeasurableItems.CreatorLink measurable />
          <MeasurableItems.ResolutionEndpoint measurable />
          <MeasurableItems.EndpointResponse measurable />
          <MeasurableItems.QuestionLink measurable />
        </Div>
      </Div>
      <MeasurableItems.Description measurable styles=[Styles.description] />
    </Div>
    <MeasurementForm
      measurable
      measurableId={measurable.id}
      isCreator={measurable.iAmOwner == Some(true)}
    />
  </>;
};