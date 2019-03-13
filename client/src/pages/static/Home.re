open Utils;
open Rationale.Result.Infix;
open Rationale;
open E;

let component = ReasonReact.statelessComponent("Home");

let make = _children => {
  ...component,
  render: _ =>
    <div className=StaticStyles.body>
      <div className=StaticStyles.title> {"Foretold" |> ste} </div>
      <div className=StaticStyles.description>
        {
          "Foretold is currently in development. Contact Ozzie for early access."
          |> ste
        }
      </div>
    </div>,
};