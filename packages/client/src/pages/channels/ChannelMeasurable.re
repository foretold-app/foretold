let component = ReasonReact.statelessComponent("ChannelMeasurable");

let make =
    (
      ~measurableId: string,
      ~channelId: option(string),
      ~loggedUser: Types.user,
      _children,
    ) => {
  ...component,
  render: _ => {
    <>
      <SLayout isFluid=true> <Measurable id=measurableId /> </SLayout>
      <MeasurableBottomSection measurableId channelId />
    </>;
  },
};
