let component = ReasonReact.statelessComponent("ChannelMeasurable");

let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    <>
      {SLayout.LayoutConfig.make(
         ~head=ReasonReact.null,
         ~body=
           <FC.PageCard.Body>
             <MeasurableFullPresentation id=measurableId loggedInUser />
           </FC.PageCard.Body>,
       )
       |> SLayout.FullPage.makeWithEl}
      <Measurements measurableId loggedInUser />
    </>;
  },
};