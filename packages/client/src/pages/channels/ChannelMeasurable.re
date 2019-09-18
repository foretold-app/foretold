let component = ReasonReact.statelessComponent("ChannelMeasurable");

let make =
    (
      ~measurableId: string,
      ~channelId: option(string),
      ~loggedInUser: Types.user,
      ~subTab: Routing.ChannelPage.measurementsTab,
      _children,
    ) => {
  ...component,
  render: _ => {
    <>
      <SLayout> <Measurable id=measurableId loggedInUser /> </SLayout>
      <Switcher measurableId channelId loggedInUser subTab />
    </>;
  },
};