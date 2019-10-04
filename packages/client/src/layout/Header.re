open Utils;
open Style.Grid;

let feedbackUrl = "https://feedback.foretold.io/";

module Styles = {
  open Css;
  let outer =
    style([
      padding2(~v=`em(0.6), ~h=`em(2.)),
      float(`left),
      backgroundColor(`rgb((255, 255, 255))),
      width(`percent(100.)),
      borderBottom(`px(2), `solid, `rgb((234, 234, 234))),
    ]);
  let left =
    style([
      flex(`num(1.)),
      backgroundColor(`hex("2c436e")),
      minWidth(`px(200)),
    ]);
  let right = style([flex(`num(7.)), width(`percent(100.0))]);
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

module StylesDropdown = {
  open Css;

  let dropdown =
    style([
      border(`px(1), `solid, `hex("d5d2d2")),
      padding2(~v=`em(0.5), ~h=`em(0.)),
      borderRadius(`px(5)),
      background(`hex("fff")),
      boxShadow(
        ~x=`px(1),
        ~y=`px(1),
        ~blur=`px(5),
        ~spread=`px(1),
        ~inset=false,
        `hex("dfd7d7"),
      ),
      maxWidth(`em(17.)),
    ]);

  let actions = [maxWidth(`em(17.))] |> style;

  let clear = [clear(`both)] |> style;

  let action =
    style([
      cursor(`pointer),
      padding2(~v=`em(0.3), ~h=`em(1.4)),
      minWidth(`px(200)),
      fontSize(`em(1.2)),
      width(`percent(100.)),
      float(`left),
      color(`hex("666")),
      selector(
        ":hover",
        [color(`hex("666")), backgroundColor(`hex("eef0f3"))],
      ),
    ]);
};

let action = StylesDropdown.action;

let link = (linkType: LinkType.linkType, str) =>
  <Link.Jsx2 linkType className=action> {str |> ste} </Link.Jsx2>;

let userDropdown = agentId =>
  <div className=StylesDropdown.actions>
    {link(Internal(Agent({agentId, subPage: AgentUpdates})), "My Profile")}
    {link(Internal(Agent({agentId, subPage: AgentBots})), "My Bots")}
    {link(Internal(Profile), "User Settings")}
    {link(Internal(Preferences), "User Preferences")}
    {link(Internal(ChannelNew), "Make a New Community")}
    {link(External(feedbackUrl), "Feedback")}
    {link(Action(_ => Auth.Actions.logout()), "Logout")}
    <div className=StylesDropdown.clear />
  </div>;

let header = (loggedInUser: Types.user) =>
  switch (loggedInUser.agent) {
  | Some((agent: Types.agent)) =>
    <AntdDropdown
      overlay={userDropdown(agent.id)}
      overlayClassName=StylesDropdown.dropdown>
      {agent.name
       |> (e => e == Some("") ? None : e)
       |> E.O.default("Settings (Please add a Username on the Profile page)")
       |> ste}
      <Icon.Icon icon="CHEVRON_DOWN" />
    </AntdDropdown>
  | None => ReasonReact.null
  };

let component = ReasonReact.statelessComponent("Header");
let make = (~loggedInUser: Types.user, _children) => {
  ...component,
  render: _self =>
    <Div styles=[Styles.outer]>
      <Div float=`left>
        {Primary.User.show(
           loggedInUser,
           <Link.Jsx2
             linkType={Internal(EntityIndex)} className=Styles.headerLink>
             {"Entity Explorer" |> ste}
           </Link.Jsx2>,
         )}
      </Div>
      <Div float=`left> <VerificationWarning /> </Div>
      <Div float=`right> {header(loggedInUser)} </Div>
    </Div>,
};
