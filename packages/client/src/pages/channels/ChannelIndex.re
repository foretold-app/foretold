[@react.component]
let make = (~title="Communities") => {
  let head = <SLayout.TextDiv text=title />;
  let body = <ChannelTable isArchived=[|Some(`FALSE)|] />;

  <SLayout head> body </SLayout>;
};