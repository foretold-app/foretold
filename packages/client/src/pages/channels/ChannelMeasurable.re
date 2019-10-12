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
      <SLayout isFluid=true>
        <Measurable id=measurableId loggedInUser />
      </SLayout>
      <MeasurableBottomSection measurableId channelId loggedInUser />
    </>;
  },
};