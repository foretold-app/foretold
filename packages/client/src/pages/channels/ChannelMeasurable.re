[@react.component]
let make = (~measurableId: string, ~channelId: option(string)) => {
  <>
    <SLayout isFluid=true> <MeasurablePage id=measurableId /> </SLayout>
    <MeasurableBottomSection measurableId channelId />
  </>;
};