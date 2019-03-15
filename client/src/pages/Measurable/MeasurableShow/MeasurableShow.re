let component = ReasonReact.statelessComponent("Measurable");

let make = (~channel: string, ~id: string, ~loggedInUser, _children) => {
  ...component,
  render: _self =>
    <div>
      {SLayout.channelHeader(Some(channel))}
      <div className=SLayout.Styles.mainSection>
        <MeasurableShow__Component id loggedInUser />
      </div>
    </div>,
};