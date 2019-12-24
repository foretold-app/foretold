[@react.component]
let make = () => {
  <SLayout head={<SLayout.TextDiv text="Create a New Community" />}>
    <FC.PageCard.BodyPadding> <ChannelForm.Create /> </FC.PageCard.BodyPadding>
  </SLayout>;
};