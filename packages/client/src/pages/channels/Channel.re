open SLayout;
open Style.Grid;

[@react.component]
let make =
    (
      ~channelPage: Routing.ChannelPage.t,
      ~channel: option(Types.channel),
      ~children=<Null />,
    ) => {
  let channelId = channelPage.channelId;

  module TopOrdinaryChannel = {
    let (showFullDescription, setShowFullDescription) =
      React.useState(() => false);

    let toggleClass =
      Css.(
        style([
          color(`hex("485bc1")),
          cursor(`pointer),
          marginTop(`em(-0.9)),
        ])
      );

    let maxLength = 130;

    [@react.component]
    let make = (~channel: Types.channel) => {
      module JoinButton = {
        [@react.component]
        let make = (~channelId) =>
          E.React2.showIf(
            Primary.Permissions.can(`JOIN_CHANNEL, channel.permissions),
            <SimpleHeader.JoinChannel channelId />,
          );
      };

      module LeaveButton = {
        [@react.component]
        let make = (~channelId) =>
          E.React2.showIf(
            Primary.Permissions.can(`LEAVE_CHANNEL, channel.permissions),
            <SimpleHeader.LeaveChannel channelId />,
          );
      };

      module BookmarkButton = {
        [@react.component]
        let make = (~channelId) =>
          E.React2.showIf(
            Primary.Permissions.can(
              `CHANNEL_BOOKMARK_TOGGLE,
              channel.permissions,
            ),
            <SimpleHeader.BookmarkChannel channel />,
          );
      };

      <>
        <Div float=`left flexDirection=`column>
          <Div flex={`num(1.0)}> <ChannelLink channel /> </Div>
          {channel.description
           |> E.O.React.fmapOrNull(source =>
                <Div
                  flex={`num(1.0)}
                  styles=[Css.(style([marginTop(`em(0.5))]))]>
                  <Markdown
                    source={
                      showFullDescription
                        ? source : Utils.truncateByWords(~maxLength, source)
                    }
                  />
                  {E.React2.showIf(
                     !showFullDescription
                     && Utils.truncateByWords(~maxLength, source) != source,
                     <div
                       onClick={_ => setShowFullDescription(_ => true)}
                       className=toggleClass>
                       {"(more)" |> Utils.ste}
                     </div>,
                   )}
                  {E.React2.showIf(
                     showFullDescription
                     && Utils.truncateByWords(~maxLength, source) != source,
                     <div
                       onClick={_ => setShowFullDescription(_ => false)}
                       className=toggleClass>
                       {"close" |> Utils.ste}
                     </div>,
                   )}
                </Div>
              )}
        </Div>
        <Div float=`right>
          {channel.myRole === Some(`NONE)
             ? <JoinButton channelId={channel.id} />
             : <LeaveButton channelId={channel.id} />}
        </Div>
        <Div float=`right> <BookmarkButton channelId={channel.id} /> </Div>
      </>;
    };
  };

  module Top = {
    [@react.component]
    let make = (~channel) =>
      switch (channelId) {
      | "" => <Null />
      | _ =>
        <ForetoldComponents.GroupHeader>
          <TopOrdinaryChannel channel />
        </ForetoldComponents.GroupHeader>
      };
  };

  module Headers = {
    [@react.component]
    let make = () => {
      let tabSelected =
        Routing.ChannelPage.SubPage.toTab(channelPage.subPage);

      switch (channel) {
      | Some(channel) =>
        <>
          <Top channel />
          <ForetoldComponents.GroupHeader.SubHeader>
            <ChannelTabs tabSelected channel />
          </ForetoldComponents.GroupHeader.SubHeader>
        </>
      | _ => <div />
      };
    };
  };

  <FillWithSidebar channelId={Some(channelId)}>
    <Headers />
    children
  </FillWithSidebar>;
};