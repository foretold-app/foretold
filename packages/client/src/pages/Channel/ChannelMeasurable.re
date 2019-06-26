let component = ReasonReact.statelessComponent("ChannelMeasurable");

let make =
    (
      ~measurableId: string,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    SLayout.LayoutConfig.make(
      ~head=ReasonReact.null,
      ~body=
        <FC.PageCard.Body>
          <C.Measurable.FullPresentation id=measurableId loggedInUser />
        </FC.PageCard.Body>,
    )
    |> layout;
  },
};