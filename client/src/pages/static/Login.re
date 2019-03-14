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
        <Antd.Button _type=`primary onClick={_e => Auth0.logIn()}>
          {"Login" |> ste}
        </Antd.Button>
      </div>
    </div>,
};