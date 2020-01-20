[@react.component]
let make = () => {
  <SLayout head={<SLayout.TextDiv text="Create a New Community" />}>
    <ForetoldComponents.PageCard.BodyPadding>
      <ChannelForm.Create />
    </ForetoldComponents.PageCard.BodyPadding>
  </SLayout>;
};