[@bs.config {jsx: 3}];

[@react.component]
let make = (~layout, ~title) =>
  SLayout.LayoutConfig.make(
    ~head=SLayout.Header.textDiv(title),
    ~body=<ChannelTable isArchived=[|Some(`FALSE)|] />,
  )
  |> layout;

module Jsx2 = {
  let component = ReasonReact.statelessComponent("ChannelIndex");
  let make =
      (~layout=SLayout.FullPage.makeWithEl, ~title="Communities", children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~layout, ~title, ()),
      children,
    );
};