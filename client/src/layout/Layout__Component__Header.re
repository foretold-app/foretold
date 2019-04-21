open Utils;
open Style.Grid;
open Foretold__GraphQL;

module Styles = {
  open Css;
  let outer =
    style([
      padding2(~v=`em(1.5), ~h=`em(2.)),
      float(`left),
      backgroundColor(`rgb((255, 255, 255))),
      width(`percent(100.)),
      borderBottom(`px(2), `solid, `rgb((234, 234, 234))),
    ]);
  let left =
    style([
      flex(1),
      backgroundColor(`hex("2c436e")),
      minWidth(`px(200)),
    ]);
  let right = style([flex(7), width(`percent(100.0))]);
};

let foo: ReasonReact.reactElement = "sdf" |> ste;

let component = ReasonReact.statelessComponent("Header");

let bar = agentId =>
  <Antd.Menu>
    <Antd.Menu.Item>
      <div onClick={_ => Context.Routing.Url.push(Profile)}>
        {"Profile" |> ste}
      </div>
    </Antd.Menu.Item>
    <Antd.Menu.Item>
      <div
        onClick={_ => Context.Routing.Url.push(AgentMeasurables(agentId))}>
        {"My Questions" |> ste}
      </div>
    </Antd.Menu.Item>
    <Antd.Menu.Item>
      <div onClick={_ => Context.Routing.Url.push(AgentBots(agentId))}>
        {"My Bots" |> ste}
      </div>
    </Antd.Menu.Item>
    <Antd.Menu.Item>
      <div onClick={_ => Context.Routing.Url.push(AgentShow(agentId))}>
        {"My Predictions" |> ste}
      </div>
    </Antd.Menu.Item>
    <Antd.Menu.Item>
      <div onClick={_ => Context.Auth.Actions.logout()}>
        {"Log Out" |> ste}
      </div>
    </Antd.Menu.Item>
  </Antd.Menu>;

let header = (loggedInUser: Context.Primary.User.t) => {
  let id =
    loggedInUser.agent |> E.O.fmap((a: Context.Primary.Agent.t) => a.id);
  switch (id) {
  | Some(id) =>
    <AntdDropdown2 overlay={bar(id)}> {"OzzieGooen" |> ste} </AntdDropdown2>
  | None => ReasonReact.null
  };
};
let make = (~loggedInUser: Context.Primary.User.t, _children) => {
  ...component,
  render: _self =>
    <Div styles=[Styles.outer]>
      <Div float=`left>
        <div onClick={_e => Context.Routing.Url.push(EntityIndex)}>
          {"Entity List" |> ste}
        </div>
      </Div>
      <Div float=`right> {header(loggedInUser)} </Div>
    </Div>,
};