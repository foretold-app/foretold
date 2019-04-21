open Utils;
open Foretold__GraphQL;

module Styles = {
  open Css;
  let sidebar = style([display(`flex), flexDirection(`column)]);
  let over = style([display(`flex), flexDirection(`column)]);
  let hash =
    style([marginRight(`px(5)), color(`rgba((255, 255, 255, 0.3)))]);
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
      flex(1),
      color(`rgba((255, 255, 255, 0.6))),
      hover([color(`rgba((255, 255, 255, 0.6)))]),
      fontSize(`em(1.2)),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
      cursor(`pointer),
      textDecoration(`none),
      hover([background(`hex("435e90"))]),
      selector("a", [borderBottom(`px(2), `solid, hex("eee"))]),
    ]);
  let selectedItem =
    style([
      flex(1),
      color(`rgba((255, 255, 255, 0.8))),
      hover([color(`rgba((255, 255, 255, 0.8)))]),
      background(`hex("3192ff")),
      fontSize(`em(1.2)),
      cursor(`pointer),
      padding4(~top=`px(4), ~bottom=`px(4), ~left=`px(14), ~right=`px(2)),
      focus([textDecoration(`none)]),
      textDecoration(`none),
    ]);
};

let component = ReasonReact.statelessComponent("Sidebar");
let make = (~channelId, ~loggedInUser: Context.Primary.User.t, _children) => {
  ...component,
  render: _self =>
    <div className=Styles.sidebar>
      <div className=Styles.over />
      <div className=Styles.sectionPadding />
      <div className=Styles.minorHeader>
        <div
          className=Styles.minorHeaderLink
          onClick={_e => Context.Routing.Url.push(ChannelIndex)}>
          {"Channels" |> ste}
        </div>
        <div
          className=Styles.minorHeaderLinkPlus
          onClick={_e => Context.Routing.Url.push(ChannelNew)}>
          <Icon.Icon icon="CIRCLE_PLUS" />
        </div>
      </div>
      <div className=Styles.over>
        {
          loggedInUser.agent
          |> E.O.fmap((r: Context.Primary.Agent.t) =>
               r.channelMemberships
               |> E.A.O.defaultEmpty
               |> E.A.fmap((r: Context.Primary.Types.channelMembership) =>
                    r.channel
                  )
               |> E.A.O.concatSomes
               |> E.A.fmap((channel: Context.Primary.Channel.t) => {
                    let _channel: Context.Primary.Channel.t =
                      Context.Primary.Channel.make(
                        ~id=channel.id,
                        ~name=channel.name,
                        ~isArchived=false,
                        ~isPublic=channel.isPublic,
                        (),
                      );
                    <div
                      onClick={
                        _e => Context.Primary.Channel.showPush(_channel)
                      }
                      className={
                        Some(_channel.id) == channelId ?
                          Styles.selectedItem : Styles.item
                      }>
                      {
                        Context.Primary.Channel.present(
                          ~hashClassName=Styles.hash,
                          _channel,
                        )
                      }
                    </div>;
                  })
               |> ReasonReact.array
             )
          |> E.O.React.defaultNull
        }
      </div>
    </div>,
};