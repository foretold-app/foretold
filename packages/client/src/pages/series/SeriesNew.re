[@react.component]
let make = (~channelId: string) => {
  <SLayout head={<SLayout.TextDiv text="Make a New Series" />}>
    <FC.PageCard.BodyPadding>
      <SeriesForm.Create channelId />
    </FC.PageCard.BodyPadding>
  </SLayout>;
};