open Utils;

let component = ReasonReact.statelessComponent("Home");

let make = _children => {
  ...component,
  render: _ =>
    <div className=StaticStyles.body>
      <div className=StaticStyles.title> {"Foretold" |> ste} </div>
      <div className=StaticStyles.description>
        {"Foretold is currently in private beta. Contact  " |> ste}
        <a href="mailto:ozzieagooen@gmail.com"> {"Ozzie Gooen" |> ste} </a>
        {" for early access." |> ste}
      </div>
    </div>,
};
