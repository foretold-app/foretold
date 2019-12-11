[@react.component]
let make = (~channelId: string) => {
  <SLayout head={SLayout.Header.textDiv("Make a New Notebook")} isFluid=true>
    <NotebookForm.Create channelId />
  </SLayout>;
};