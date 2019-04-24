open Utils;
open Style.Grid;
open Foretold__GraphQL;

/* border: 1px solid #dcdcdc;
       box-shadow: 1px 1px 5px #ede6e6;
   } */
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
  let headerLink =
    style([
      cursor(`pointer),
      marginRight(`em(2.)),
      fontSize(`em(1.1)),
      color(`hex("61738d")),
      fontWeight(`num(400)),
      selector(":hover", [color(`hex("000"))]),
    ]);
};

let foo: ReasonReact.reactElement = "sdf" |> ste;

let component = ReasonReact.statelessComponent("Header");

let action = Layout__Dropdown.Styles.action;
let bar = agentId =>
  <>
    <div onClick={_ => Context.Routing.Url.push(Profile)} className=action>
      {"Profile" |> ste}
    </div>
    <div
      onClick={_ => Context.Routing.Url.push(AgentMeasurables(agentId))}
      className=action>
      {"My Questions" |> ste}
    </div>
    <div
      onClick={_ => Context.Routing.Url.push(AgentBots(agentId))}
      className=action>
      {"My Bots" |> ste}
    </div>
    <div
      onClick={_ => Context.Routing.Url.push(AgentShow(agentId))}
      className=action>
      {"My Predictions" |> ste}
    </div>
    <div
      onClick={_e => Context.Routing.Url.push(ChannelNew)} className=action>
      {"Make a New Channel" |> ste}
    </div>
    <div onClick={_ => Context.Auth.Actions.logout()} className=action>
      {"Log Out" |> ste}
    </div>
  </>;

let header = (loggedInUser: Context.Primary.User.t) =>
  switch (loggedInUser.agent) {
  | Some((agent: Context.Primary.Types.agent)) =>
    <AntdDropdown
      overlay={bar(agent.id)}
      overlayClassName=Layout__Dropdown.Styles.dropdown>
      {agent.name |> E.O.default("") |> ste}
      <Icon.Icon icon="CHEVRON_DOWN" />
    </AntdDropdown>
  | None => ReasonReact.null
  };
let make = (~loggedInUser: Context.Primary.User.t, _children) => {
  ...component,
  render: _self =>
    <Div styles=[Styles.outer]>
      <Div float=`left>
        <div
          onClick={_e => Context.Routing.Url.push(ChannelIndex)}
          className=Styles.headerLink>
          {"Public Channels" |> ste}
        </div>
      </Div>
      <Div float=`left>
        <div
          onClick={_e => Context.Routing.Url.push(EntityIndex)}
          className=Styles.headerLink>
          {"Entity Explorer" |> ste}
        </div>
      </Div>
      <Div float=`right> {header(loggedInUser)} </Div>
    </Div>,
};