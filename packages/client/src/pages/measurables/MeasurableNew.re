[@bs.config {jsx: 3}];

[@react.component]
let make = (~channelId) => {
  <SLayout head={SLayout.Header.textDiv("New Question")}>
    <FC.PageCard.BodyPadding>
      <MeasurableForm.Create channelId />
    </FC.PageCard.BodyPadding>
  </SLayout>;
};