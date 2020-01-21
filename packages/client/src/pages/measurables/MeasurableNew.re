[@react.component]
let make = (~channelId) => {
  <SLayout head={<SLayout.TextDiv text="New Question" />}>
    <ForetoldComponents.PageCard.BodyPadding>
      <MeasurableForm.Create channelId />
    </ForetoldComponents.PageCard.BodyPadding>
  </SLayout>;
};