let component = ReasonReact.statelessComponent("Measurable");

let make = (~id: string, ~userQuery, _children) => {
  ...component,
  render: _self => <MeasurableShow__Component id userQuery />,
};