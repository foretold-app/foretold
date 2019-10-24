[@bs.config {jsx: 3}];

open Utils;

module Styles = {
  open Css;
  let sidebar = style([display(`flex), flexDirection(`column)]);
  let over = style([display(`flex), flexDirection(`column)]);
  let hash =
    style([
      marginRight(`px(5)),
      color(`rgba((255, 255, 255, 0.3))),
      float(`left),
    ]);
  let minorHeader =
    style([
      color(`rgba((255, 255, 255, 0.6))),
      fontSize(`em(1.15)),
      width(`percent(100.)),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
    ]);
  let minorHeaderLink =
    style([
      float(`left),
      color(`rgba((255, 255, 255, 0.6))),
      cursor(`pointer),
      selector(":hover", [color(`rgba((255, 255, 255, 0.9)))]),
    ]);
  let minorHeaderLinkPlus =
    style([
      float(`right),
      color(`rgba((255, 255, 255, 0.6))),
      cursor(`pointer),
      paddingRight(`em(0.3)),
      marginTop(`em(-0.2)),
      selector(":hover", [color(`rgba((255, 255, 255, 0.9)))]),
    ]);
  let sectionPadding = style([height(`em(1.0)), width(`percent(100.0))]);
  let item =
    style([
      flex(`num(1.)),
      color(`rgba((255, 255, 255, 0.6))),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
      cursor(`pointer),
      textDecoration(`none),
      hover([background(`hex("435e90"))]),
      selector("a", [borderBottom(`px(2), `solid, hex("eee"))]),
      selector(
        ":hover",
        [background(`hex("435e90")), color(`rgba((255, 255, 255, 0.6)))],
      ),
    ]);
  let selectedItem =
    style([
      flex(`num(1.)),
      color(`rgba((255, 255, 255, 0.8))),
      background(`hex("3192ff")),
      cursor(`pointer),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
      focus([textDecoration(`none)]),
      textDecoration(`none),
      selector(":hover", [color(`rgba((255, 255, 255, 0.8)))]),
    ]);
};

module ChannelsList = {
  [@react.component]
  let make = (~channelId, ~loggedUser: Types.user) => {
    loggedUser.agent
    |> E.O.fmap((agent: Types.agent) =>
         ChannelsGet.component(
           ~channelMemberId=?Some(agent.id),
           ~sortFn=ChannelsGet.sortAsc,
           channels =>
           channels
           |> Array.mapi((index, channel: Types.channel) =>
                <Link
                  key={index |> string_of_int}
                  linkType={Internal(Primary.Channel.showLink(channel))}
                  className={
                    Some(channel.id) == channelId
                      ? Styles.selectedItem : Styles.item
                  }>
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
    <div className=Styles.over />
    <div className=Styles.minorHeader>
      <Link
        linkType={Internal(ChannelIndex)} className=Styles.minorHeaderLink>
        {"Communities" |> ste}
      </Link>
    </div>
    <div className=Styles.over>
      <Link
        key="channel-global-item"
        linkType={Internal(Primary.Channel.globalLink())}
        className={
          Some("home") == channelId ? Styles.selectedItem : Styles.item
        }>
        {Primary.Channel.presentGlobal(~className=Styles.hash, ())}
      </Link>
      {loggedUser
       |> E.O.React.fmapOrNull(loggedUser =>
            <ChannelsList loggedUser channelId />
          )}
    </div>
  </div>;
};
