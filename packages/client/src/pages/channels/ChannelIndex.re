[@bs.config {jsx: 3}];

[@react.component]
let make = (~layout, ~title) =>
  SLayout.LayoutConfig.make(
    ~head=SLayout.Header.textDiv(title),
    ~body=<ChannelTable isArchived=[|Some(`FALSE)|] />,
    (),
  )
  |> layout;
