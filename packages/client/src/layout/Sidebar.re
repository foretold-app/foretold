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
      fontSize(`em(1.4)),
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
      fontSize(`em(1.1)),
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
      fontSize(`em(1.1)),
      cursor(`pointer),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
      focus([textDecoration(`none)]),
      textDecoration(`none),
      selector(":hover", [color(`rgba((255, 255, 255, 0.8)))]),
    ]);
};

let component = ReasonReact.statelessComponent("Sidebar");
let make = (~channelId, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _self =>
    <div className=Styles.sidebar>
      <div className=Styles.over />
      <div className=Styles.sectionPadding />
      <div className=Styles.minorHeader>
        <Link.Jsx2
          linkType={Internal(ChannelIndex)} className=Styles.minorHeaderLink>
          {"Communities" |> ste}
        </Link.Jsx2>
      </div>
      <div className=Styles.over>
        <Link.Jsx2
          key="channel-global-item"
          linkType={Internal(Primary.Channel.globalLink())}
          className={
            Some("home") == channelId ? Styles.selectedItem : Styles.item
          }>
          {Primary.Channel.presentGlobal(~className=Styles.hash, ())}
        </Link.Jsx2>
        {loggedInUser.agent
         |> E.O.fmap((agent: Types.agent) =>
              ChannelsGet.component(
                ~channelMemberId=?Some(agent.id),
                ~sortFn=ChannelsGet.sortAsc,
                channels =>
                channels
                |> Array.mapi((index, channel: Types.channel) =>
                     <Link.Jsx2
                       key={index |> string_of_int}
                       linkType={Internal(Primary.Channel.showLink(channel))}
                       className={
                         Some(channel.id) == channelId
                           ? Styles.selectedItem : Styles.item
                       }>
                       {Primary.Channel.present(
                          ~className=Styles.hash,
                          channel,
                        )}
                     </Link.Jsx2>
                   )
                |> ReasonReact.array
              )
            )
         |> E.O.React.defaultNull}
      </div>
    </div>,
};