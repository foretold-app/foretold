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

      <>
        <Div float=`left flexDirection=`column>
          <Div flex={`num(1.0)}> <ChannelLink channel /> </Div>
          {channel.description
           |> E.O.React.fmapOrNull(source =>
                <Div
                  flex={`num(1.0)}
                  styles=[Css.(style([marginTop(`em(0.5))]))]>
                  <Markdown source />
                </Div>
              )}
        </Div>
        <Div float=`right>
          {channel.myRole === Some(`NONE)
             ? <JoinButton channelId={channel.id} />
             : <LeaveButton channelId={channel.id} />}
        </Div>
      </>;
    };
  };

  module TopGlobalChannel = {
    [@react.component]
    let make = (~channel) => {
      <Div float=`left flexDirection=`column>
        <Div flex={`num(1.0)}> <ChannelLink channel /> </Div>
        {channel.description
         |> E.O.React.fmapOrNull(source =>
              <Div
                flex={`num(1.0)}
                styles=[
                  Css.(
                    style([
                      marginTop(`em(0.5)),
                      selector(
                        "p",
                        [
                          marginBottom(`zero),
                          color(ForetoldComponents.Base.Colors.textDark),
                        ],
                      ),
                    ])
                  ),
                ]>
                <Markdown source />
              </Div>
            )}
      </Div>;
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