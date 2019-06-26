open Utils;
open Style.Grid;

module Styles = {
  open Css;
  let outer =
    style([
      padding2(~v=`em(1.0), ~h=`em(2.)),
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

let component = ReasonReact.statelessComponent("Header");

let action = Layout__Dropdown.Styles.action;

let link = (linkType: C.Link.linkType, str) =>
  <C.Link linkType className=action> {str |> ste} </C.Link>;

let userDropdown = agentId =>
  <div className=Layout__Dropdown.Styles.actions>
    {link(Internal(Profile), "Profile")}
    {link(
       Internal(Agent({agentId, subPage: AgentMeasurables})),
       "My Questions",
     )}
    {link(Internal(Agent({agentId, subPage: AgentBots})), "My Bots")}
    {link(
       Internal(Agent({agentId, subPage: AgentMeasurements})),
       "My Predictions",
     )}
    {link(Internal(ChannelNew), "Make a New Channel")}
    {link(Action(_ => Context.Auth.Actions.logout()), "Logout")}
    <div className=Layout__Dropdown.Styles.clear />
  </div>;

let header = (loggedInUser: Context.Primary.User.t) =>
  switch (loggedInUser.agent) {
  | Some((agent: Context.Primary.Types.agent)) =>
    <AntdDropdown
      overlay={userDropdown(agent.id)}
      overlayClassName=Layout__Dropdown.Styles.dropdown>
      {agent.name
       |> (e => e == Some("") ? None : e)
       |> E.O.default("Settings (Please add a Username on the Profile page)")
       |> ste}
      <Icon.Icon icon="CHEVRON_DOWN" />
    </AntdDropdown>
  | None => ReasonReact.null
  };

let make = (~loggedInUser: Context.Primary.User.t, _children) => {
  ...component,
  render: _self =>
    <Div styles=[Styles.outer]>
      <Div float=`left>
        <C.Link linkType={Internal(ChannelIndex)} className=Styles.headerLink>
          {"Public Groups" |> ste}
        </C.Link>
      </Div>
      <Div float=`left>
        <C.Link linkType={Internal(EntityIndex)} className=Styles.headerLink>
          {"Entity Explorer" |> ste}
        </C.Link>
      </Div>
      <Div float=`right> {header(loggedInUser)} </Div>
    </Div>,
};