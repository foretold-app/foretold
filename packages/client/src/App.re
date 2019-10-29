[@bs.config {jsx: 3}];

[@react.component]
let make = () => {
  let a1 = React.useState(() => "routestr");
  let (route, setRoute) = a1;

  route |> ReasonReact.string;
};