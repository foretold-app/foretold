[@bs.config {jsx: 3}];

[@react.component]
let make = () => {
  <SLayout head={SLayout.Header.textDiv("Create a New Community")}>
    <FC.PageCard.BodyPadding> <ChannelForm.Create /> </FC.PageCard.BodyPadding>
  </SLayout>;
};