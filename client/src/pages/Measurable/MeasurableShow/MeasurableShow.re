let component = ReasonReact.statelessComponent("Measurable");

let make = (~id: string, ~loggedInUser, _children) => {
  ...component,
  render: _self => <MeasurableShow__Component id loggedInUser />,
};