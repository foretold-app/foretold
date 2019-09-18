let component = ReasonReact.statelessComponent("ChannelMeasurable");

let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    <>
      <SLayout> <Measurable id=measurableId loggedInUser /> </SLayout>
      <Switcher measurableId loggedInUser />
    </>;
  },
};