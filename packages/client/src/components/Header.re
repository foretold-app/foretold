open Utils;
open Style.Grid;

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
      boxShadows([
        Shadow.box(
          ~x=px(1),
          ~y=px(1),
          ~blur=px(5),
          ~spread=px(1),
          ~inset=false,
          hex("dfd7d7"),
        ),
      ]),
      maxWidth(`em(17.)),
    ]);

  let actions = [maxWidth(`em(15.))] |> style;

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

module LinkHeader = {
  [@react.component]
  let make = (~linkType: LinkType.linkType, ~str) =>
    <Link linkType className=action> {str |> ste} </Link>;
};

module UserDropdown = {
  [@react.component]
  let make = (~agentId) =>
    <div className=StylesDropdown.actions>
      <LinkHeader
        linkType={Internal(Agent({agentId, subPage: AgentUpdates}))}
        str="Profile"
      />
      <LinkHeader linkType={Internal(Profile)} str="Settings" />
      <LinkHeader linkType={Internal(Preferences)} str="Preferences" />
      <LinkHeader linkType={Internal(ChannelNew)} str="New Community" />
      <LinkHeader
        linkType={Action(_ => Auth.Actions.logout())}
        str="Logout"
      />
      <div className=StylesDropdown.clear />
    </div>;
};

module Header = {
  [@react.component]
  let make = (~loggedUser: Types.user) =>
    switch (loggedUser.agent) {
    | Some((agent: Types.agent)) =>
      <Antd_Dropdown
        trigger=[|"hover"|]
        placement=`bottomLeft
        overlay={<UserDropdown agentId={agent.id} />}
        overlayClassName=StylesDropdown.dropdown>
        <div className=Css.style([Css.height(`em(1.5))])>
          <Div styles=[Css.style([Css.color(`hex("61738d"))])]>
            <Div
              float=`left styles=[Css.style([Css.marginLeft(`em(0.2))])]>
              {switch (agent.name) {
               | Some("")
               | None => "Please add a Username on the Profile page" |> ste
               | Some(name) => name |> ste
               }}
            </Div>
            <Div
              float=`left
              styles=[
                Css.style([
                  Css.marginLeft(`em(0.3)),
                  Css.fontSize(`em(0.7)),
                  Css.opacity(0.4),
                ]),
              ]>
              <Icon icon="CHEVRON_DOWN" />
            </Div>
            {loggedUser.picture
             |> E.O.React.fmapOrNull((picture: string) =>
                  <Div
                    float=`left
                    styles=[Css.style([Css.marginLeft(`em(0.45))])]>
                    <ForetoldComponents.Base.Avatar src=picture width=1.5 />
                  </Div>
                )}
          </Div>
        </div>
      </Antd_Dropdown>
    | None => <Null />
    };
};

[@react.component]
let make = (~loggedUser: option(Types.user)) => {
  <Div styles=[Styles.outer]>
    <Div float=`left>
      {switch (loggedUser) {
       | Some(loggedUser) =>
         Primary.User.show(
           loggedUser,
           <Link linkType={Internal(EntityIndex)} className=Styles.headerLink>
             {"Entity Explorer" |> ste}
           </Link>,
         )
       | None =>
         <Link linkType={Internal(ChannelIndex)} className=Styles.headerLink>
           {"Communities" |> ste}
         </Link>
       }}
    </Div>
    <Div float=`left> <VerificationWarning /> </Div>
    <Div float=`right>
      {switch (loggedUser) {
       | Some(loggedUser) => <Header loggedUser />
       | None =>
         <Link
           linkType={Action(_e => Auth0Client.triggerLoginScreen())}
           className=Styles.headerLink>
           {"Log In" |> ste}
         </Link>
       }}
    </Div>
  </Div>;
};