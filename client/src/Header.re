let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("User");

let link = (url, name) =>
  <a onClick=(_e => ReasonReact.Router.push(url))>
    (name |> ReasonReact.string)
  </a>;

let make = _children => {
  ...component,
  render: _ =>
    <div>
      <h4> ("Prediction App" |> ste) </h4>
      (link("/users", "Users"))
      (link("/measurables", "Measurables"))
    </div>,
};