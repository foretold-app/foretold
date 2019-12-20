[@react.component]
let make = (~channelId: string) => {
  <SLayout head={<SLayout.TextDiv text="Make a New Notebook" />} isFluid=true>
    <NotebookForm.Create channelId />
  </SLayout>;
};