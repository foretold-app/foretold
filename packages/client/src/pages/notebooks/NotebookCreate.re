[@react.component]
let make = (~channelId: string) => {
  <SLayout
    head={<SLayout.TextDiv text="Make a New Notebook" />} container=`fluid>
    <NotebookForm.Create channelId />
  </SLayout>;
};