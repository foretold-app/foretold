let component = ReasonReact.statelessComponent("ChannelMeasurable");

let make =
    (
      ~measurableId: string,
      ~channelId: option(string),
      ~loggedInUser: Types.user,
      _children,
    ) => {
  ...component,
  render: _ => {
    <>
      <SLayout> <Measurable id=measurableId loggedInUser /> </SLayout>
      <MeasurableButtom measurableId channelId loggedInUser />
    </>;
  },
};