let component = ReasonReact.statelessComponent("Measurable");

let make = (~channel: string, ~id: string, ~loggedInUser, _children) => {
  ...component,
  render: _self =>
    <>
      <C.Channel.SimpleHeader channel={Some(channel)} />
      <div className=SLayout.Styles.mainSection>
        <C.Measurable.FullPresentation id loggedInUser />
      </div>
    </>,
};