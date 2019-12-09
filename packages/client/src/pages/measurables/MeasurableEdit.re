[@bs.config {jsx: 3}];

[@react.component]
let make = (~pageParams: Types.pageParams) => {
  <SLayout head={SLayout.Header.textDiv("Edit Question")}>
    <FC.PageCard.BodyPadding>
      {MeasurableGet.component(~id=pageParams.id, measurable =>
         <MeasurableForm.Edit id={pageParams.id} measurable />
       )}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};