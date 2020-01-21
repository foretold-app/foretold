[@react.component]
let make = (~channelId: string) => {
  <SLayout head={<SLayout.TextDiv text="Make a New Series" />}>
    <ForetoldComponents.PageCard.BodyPadding>
      <SeriesForm.Create channelId />
    </ForetoldComponents.PageCard.BodyPadding>
  </SLayout>;
};