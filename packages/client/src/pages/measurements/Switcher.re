let component = ReasonReact.statelessComponent("Switcher");
let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    <>
      <Measurements measurableId loggedInUser />
      {"A" |> Utils.ste}
      <Scores measurableId loggedInUser />
    </>;
  },
};