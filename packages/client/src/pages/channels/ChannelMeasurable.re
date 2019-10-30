[@bs.config {jsx: 3}];

[@react.component]
let make = (~measurableId: string, ~channelId: option(string)) => {
  <>
    <SLayout isFluid=true> <Measurable id=measurableId /> </SLayout>
    <MeasurableBottomSection measurableId channelId />
  </>;
};
