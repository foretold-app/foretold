[@bs.config {jsx: 3}];

let a1 = React.useState(() => "routestr");

[@react.component]
let make = () => {
  let (route, setRoute) = a1;

  route |> ReasonReact.string;
};