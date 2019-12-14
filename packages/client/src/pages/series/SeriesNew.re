[@react.component]
let make = (~channelId: string) => {
  <SLayout head={SLayout.Header.textDiv("Make a New Series")}>
    <FC.PageCard.BodyPadding>
      <SeriesForm.Create channelId />
    </FC.PageCard.BodyPadding>
  </SLayout>;
};