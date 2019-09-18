let component = ReasonReact.statelessComponent("ChannelMeasurable");

let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    <>
      <SLayout>
        <MeasurableFullPresentation id=measurableId loggedInUser />
      </SLayout>
      <Measurements measurableId loggedInUser />
    </>;
  },
};