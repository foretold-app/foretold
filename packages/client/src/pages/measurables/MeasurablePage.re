open Style.Grid;

module Styles = {
  open Css;
  let header =
    style(
      [padding2(~v=`em(1.5), ~h=`em(1.5))]
      @ ForetoldComponents.Base.BaseStyles.fullWidthFloatLeft,
    );
  let link = style([marginTop(em(1.))]);

  let description = style([paddingTop(`em(1.5))]);
};

[@react.component]
let make = (~measurable: Types.measurable) => {
  <>
    <Div styles=[Styles.header]>
      <Div flexDirection=`row>
        <Div flex={`num(3.)}>
          <ForetoldComponents.PageCard.H1>
            <MeasurableItems.LinkMeasurable measurable />
          </ForetoldComponents.PageCard.H1>
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
    <MeasurementForm measurable key={measurable.id} />
  </>;
};