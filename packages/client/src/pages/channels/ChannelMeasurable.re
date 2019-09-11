let component = ReasonReact.statelessComponent("ChannelMeasurable");

let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    <>
      {SLayout.LayoutConfig.make(
         ~head=ReasonReact.null,
         ~body=
           <Fc.FC.PageCard.Body>
             <MeasurableFullPresentation id=measurableId loggedInUser />
           </Fc.FC.PageCard.Body>,
       )
       |> SLayout.FullPage.makeWithEl}
      <Measurements measurableId loggedInUser />
    </>;
  },
};