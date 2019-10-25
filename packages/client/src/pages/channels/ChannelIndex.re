[@bs.config {jsx: 3}];

[@react.component]
let make = (~title="") => {
  let head = SLayout.Header.textDiv(title);
  let body = <ChannelTable isArchived=[|Some(`FALSE)|] />;

  <SLayout head isFluid=false> body </SLayout>;
};
