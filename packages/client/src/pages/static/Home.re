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
        <Link.Jsx2 linkType={Internal(Privacy)}>
          {"here, " |> ste}
        </Link.Jsx2>
        {"and our Terms of Service is " |> ste}
        <Link.Jsx2 linkType={Internal(Terms)}> {"here" |> ste} </Link.Jsx2>
        {"." |> ste}
      </div>
    </div>,
};