open Utils;

module FC = ForetoldComponents;
module Styles = {
  open Css;
  let sidebar = style([display(`flex), flexDirection(`column)]);
  let over = style([display(`flex), flexDirection(`column)]);
  let hash =
    style([
      marginRight(`px(5)),
      color(`rgba((0, 0, 0, 0.3))),
      float(`left),
    ]);
  let itemPadding =
    padding4(
      ~top=`em(0.3),
      ~bottom=`em(0.3),
      ~left=`em(1.),
      ~right=`em(1.),
    );
  let sectionPadding = style([height(`em(1.0)), width(`percent(100.0))]);
  let item =
    style([
      flex(`num(1.)),
      color(`rgba((0, 0, 0, 0.8))),
      itemPadding,
      cursor(`pointer),
      textDecoration(`none),
      hover([background(FC.Base.Colors.buttonHover)]),
      selector("a", [borderBottom(`px(2), `solid, hex("eee"))]),
      selector(
        ":hover",
        [
          background(FC.Base.Colors.buttonHover),
          color(`rgba((0, 0, 0, 0.6))),
        ],
      ),
    ]);
};

module ChannelsList = {
  [@react.component]
  let make = (~channelId, ~loggedUser: Types.user) => {
    loggedUser.agent
    |> E.O.fmap((agent: Types.agent) =>
         ChannelsGet.component(
           ~channelMemberId=?Some(agent.id),
           ~order=ChannelsGet.orderAsSidebar,
           channels =>
           channels
           |> Array.mapi((index, channel: Types.channel) =>
                <Link
                  key={index |> string_of_int}
                  linkType={Internal(Primary.Channel.showLink(channel))}
                  className=Styles.item>
                  {Primary.Channel.present(~className=Styles.hash, channel)}
                </Link>
              )
           |> ReasonReact.array
         )
       )
    |> E.O.React.defaultNull;
  };
};

[@react.component]
let make = (~channelId, ~loggedUser: option(Types.user)) => {
  <div className=Styles.sidebar>
    <div className=Styles.over>
      <Link
        key="channel-global-item"
        linkType={Internal(Primary.Channel.globalLink())}
        className=Styles.item>
        {Primary.Channel.presentGlobal(~className=Styles.hash, ())}
      </Link>
      <Link
        key="channel-community-list"
        linkType={Internal(ChannelIndex)}
        className=Styles.item>
        {Primary.Channel.presentCommunities(~className=Styles.hash, ())}
      </Link>
      {loggedUser
       |> E.O.React.fmapOrNull(loggedUser =>
            <ChannelsList loggedUser channelId />
          )}
    </div>
  </div>;
};