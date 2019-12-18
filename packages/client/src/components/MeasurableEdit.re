[@react.component]
let make = (~measurableId) => {
  MeasurableGet.component(~id=measurableId, measurable =>
    <MeasurableForm.Edit id=measurableId measurable />
  );
};