[@react.component]
let make = (~title="Communities") => {
  let head = SLayout.Header.textDiv(title);
  let body = <ChannelTable isArchived=[|Some(`FALSE)|] />;

  <SLayout head isFluid=false> body </SLayout>;
};