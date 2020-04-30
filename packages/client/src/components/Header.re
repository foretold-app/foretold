open Utils;
open Style.Grid;

let colorBackground = `hex("2a456c");
let linkColor = `hex("ffffffcf");
module Styles = {
  open Css;
  let outer =
    style([
      padding2(~v=`em(0.2), ~h=`em(2.)),
      float(`left),
      backgroundColor(colorBackground),
      width(`percent(100.)),
      borderBottom(`px(1), `solid, `rgb((234, 234, 234))),
    ]);
  let button =
    style([
      padding2(~v=`em(0.4), ~h=`em(0.5)),
      float(`left),
      cursor(`pointer),
      borderRadius(`px(2)),
      fontSize(`em(1.1)),
      color(linkColor),
      fontWeight(`num(400)),
      hover([background(`hex("ffffff26")), color(`hex("ffffff99"))]),
    ]);
  let right = style([flex(`num(7.)), width(`percent(100.0))]);
  let headerLink =
    style([
      color(linkColor),
      fontWeight(`num(400)),
      selector(":hover", [color(`hex("ffffffff"))]),
    ]);
  let headerIcon =
    style([
      marginRight(`em(0.4)),
      fontSize(`rem(1.2)),
      marginTop(`px(-2)),
      float(`left),
      color(`hex("ffffff99")),
    ]);
};

module Logo = {
  [@bs.module] external src: string = "../assets/logos/logowhite.png";
  let component = ReasonReact.statelessComponent("Logo");

  module Styless = {
    open Css;
    let image =
      style([
        marginRight(`em(1.)),
        marginTop(`px(-1)),
        selector("img", [height(`rem(1.2))]),
      ]);
  };

  [@react.component]
  let make = () => {
    <Link
      linkType={Internal(Primary.Channel.globalLink())}
      className={Css.merge([Styles.button, Styless.image])}>
      <img src />
    </Link>;
  };
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

module MyCommunities = {
  let makeItem = (name, icon, r): ForetoldComponents.MyCommunities.item => {
    name,
    icon,
    bookmark: false,
    href: LinkType.toString(Internal(r)),
    onClick: LinkType.onClick(Internal(r)),
    onBookmark: LinkType.onClick(Internal(r)),
  };

  let backgroundBox =
    Css.(
      style([
        fontSize(`rem(1.1)),
        width(`em(20.)),
        border(`px(1), `solid, `hex("d5d2d2")),
        paddingBottom(`em(0.5)),
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
      ])
    );

  module ChannelsListItem = {
    [@react.component]
    let make = (~channel: Types.channel, ~mutation) => {
      let isBookmarked = channel.isBookmarked |> E.O.toBool2;
      let onBookmark = e => {
        ChannelBookmarkToogle.mutate(mutation, channel.id);
        ReactEvent.Synthetic.stopPropagation(e);
      };
      <ForetoldComponents.MyCommunities.ChannelItem
        item={Primary.Channel.toMyCommunitiesItem(
          channel,
          isBookmarked,
          onBookmark,
        )}
        key={channel.id}
      />;
    };
  };

  module ChannelsListItemWithMutation = {
    [@react.component]
    let make = (~channel: Types.channel) => {
      <ChannelBookmarkToogle.Mutation>
        ...{(mutation, _) => <ChannelsListItem channel mutation />}
      </ChannelBookmarkToogle.Mutation>;
    };
  };

  module ChannelsList = {
    [@react.component]
    let make = () => {
      let context = React.useContext(Providers.app);
      context.loggedUser
      |> E.O.fmap((user: Types.user) =>
           ChannelsGet.component(
             ~channelMemberId=?Some(user.agentId),
             ~order=ChannelsGet.orderSidebar,
             channels => {
               let (bookmarked, notBookmarked) =
                 channels
                 |> Belt.Array.partition(_, (c: Types.channel) =>
                      c.isBookmarked |> E.O.toBool2
                    );
               <>
                 {E.React2.showIf(
                    E.A.length(bookmarked) > 0,
                    <>
                      <ForetoldComponents.MyCommunities.Header
                        name="BOOKMARKED COMMUNITIES"
                      />
                      {bookmarked
                       |> E.A.fmap((channel: Types.channel) =>
                            <ChannelsListItemWithMutation channel />
                          )
                       |> ReasonReact.array}
                    </>,
                  )}
                 {E.React2.showIf(
                    E.A.length(notBookmarked) > 0,
                    <>
                      <ForetoldComponents.MyCommunities.Header
                        name="MY COMMUNITIES"
                      />
                      {notBookmarked
                       |> E.A.fmap((channel: Types.channel) =>
                            <ChannelsListItemWithMutation channel />
                          )
                       |> ReasonReact.array}
                    </>,
                  )}
               </>;
             },
           )
         )
      |> E.O.React.defaultNull;
    };
  };

  module Primary = {
    [@react.component]
    let make = () => {
      ForetoldComponents.(
        <div className=backgroundBox>
          <MyCommunities>
            <MyCommunities.Header name="FEEDS" />
            <MyCommunities.Item
              item={makeItem("Home", "HOME", Primary.Channel.globalLink())}
            />
            <MyCommunities.Item
              item={makeItem("All Communities", "LIST", ChannelIndex)}
            />
            <ChannelsList />
            <MyCommunities.Header name="OPTIONS" />
            <MyCommunities.Item
              item={makeItem(
                "Create a New Community",
                "CIRCLE_PLUS",
                ChannelNew,
              )}
            />
          </MyCommunities>
        </div>
      );
    };
  };

  [@react.component]
  let make = () =>
    <Antd_Dropdown
      trigger=[|"hover"|]
      placement=`bottomLeft
      overlay={<div> <Primary /> </div>}>
      <div className=Styles.button>
        <span className=Styles.headerIcon> <Icon icon="PEOPLE" /> </span>
        {"My Communities" |> Utils.ste}
      </div>
    </Antd_Dropdown>;
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
        <div className=Styles.button>
          <Div styles=[Css.style([Css.color(linkColor)])]>
            <Div float=`left>
              {loggedUser.picture
               |> E.O.React.fmapOrNull((picture: string) =>
                    <Div float=`left>
                      <ForetoldComponents.Base.Avatar src=picture width=1.3 />
                    </Div>
                  )}
            </Div>
            <Div
              float=`left styles=[Css.style([Css.marginLeft(`em(0.9))])]>
              {switch (agent.name) {
               | Some("")
               | None => "Please add a Username on the Settings page" |> ste
               | Some(name) => name |> ste
               }}
            </Div>
            <Div
              float=`left
              styles=[
                Css.(
                  style([
                    marginLeft(`em(1.5)),
                    fontSize(`em(0.7)),
                    color(linkColor),
                    opacity(0.6),
                    marginTop(`px(-2)),
                  ])
                ),
              ]>
              <Icon icon="CHEVRON_DOWN" />
            </Div>
          </Div>
        </div>
      </Antd_Dropdown>
    | None => <Null />
    };
};

[@react.component]
let make = (~loggedUser: option(Types.user)) => {
  <Div styles=[Styles.outer]>
    <Logo />
    {switch (loggedUser) {
     | Some(loggedUser) =>
       <>
         <Div float=`left> <MyCommunities /> </Div>
         <Div float=`left>
           {Primary.User.show(
              loggedUser,
              <Link linkType={Internal(EntityIndex)} className=Styles.button>
                <span className=Styles.headerIcon>
                  <Icon icon="NAVIGATE" />
                </span>
                {"Entity Explorer" |> ste}
              </Link>,
            )}
         </Div>
       </>
     | None =>
       <Link linkType={Internal(ChannelIndex)} className=Styles.button>
         <span className=Styles.headerIcon> <Icon icon="PEOPLE" /> </span>
         {"Communities" |> ste}
       </Link>
     }}
    <Div float=`left> <VerificationWarning /> </Div>
    <Div float=`right>
      {switch (loggedUser) {
       | Some(loggedUser) => <Header loggedUser />
       | None =>
         <Link
           linkType={Action(_e => Auth0Client.triggerLoginScreen())}
           className=Styles.button>
           {"Log In" |> ste}
         </Link>
       }}
    </Div>
  </Div>;
};