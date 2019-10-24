let component = ReasonReact.statelessComponent("ChannelMeasurable");

let make = (~measurableId: string, ~channelId: option(string), _children) => {
  ...component,
  render: _ => {
    <>
      <SLayout isFluid=true> <Measurable id=measurableId /> </SLayout>
      <MeasurableBottomSection measurableId channelId />
    </>;
  },
};
