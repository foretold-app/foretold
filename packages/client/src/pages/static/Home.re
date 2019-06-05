open Utils;
open E;

let component = ReasonReact.statelessComponent("Home");

let make = _children => {
  ...component,
  render: _ =>
    <div className=StaticStyles.body>
      <div className=StaticStyles.title> {"Foretold" |> ste} </div>
      <div className=StaticStyles.description>
        {"Foretold is currently in development. Contact Ozzie for early access."
         |> ste}
      </div>
      <div className=StaticStyles.description>
        {"Our Privacy Policy is " |> ste}
        <Foretold__Components__Link linkType={Internal(Privacy)}>
          {"here, " |> ste}
        </Foretold__Components__Link>
        {"and our Terms of Service is " |> ste}
        <Foretold__Components__Link linkType={Internal(Terms)}>
          {"here" |> ste}
        </Foretold__Components__Link>
        {"." |> ste}
      </div>
    </div>,
};