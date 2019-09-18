open Style.Grid;

let component = ReasonReact.statelessComponent("Switcher");
let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    <>
      <Measurements measurableId loggedInUser />
      <Scores measurableId loggedInUser />
    </>;
  },
};