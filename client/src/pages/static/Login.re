open Utils;
open E;
open Antd;

let component = ReasonReact.statelessComponent("Login");

let make = _children => {
  ...component,
  render: _ =>
    <div className=StaticStyles.body>
      <div className=StaticStyles.title> {"Foretold" |> ste} </div>
      <div className=StaticStyles.description>
        <button
          onClick={_e => Context.Auth.Auth0Client.triggerLoginScreen()}>
          {"Login" |> ste}
        </button>
      </div>
    </div>,
};