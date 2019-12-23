[@react.component]
let make = (~channelId) => {
  <SLayout head={<SLayout.TextDiv text="New Question" />}>
    <FC.PageCard.BodyPadding>
      <MeasurableForm.Create channelId />
    </FC.PageCard.BodyPadding>
  </SLayout>;
};