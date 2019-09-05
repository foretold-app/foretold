open SLayout;
open Style.Grid;

let component = ReasonReact.statelessComponent("ChannelContainer");
let make =
    (
      ~channelPage: Routing.ChannelPage.t,
      ~loggedInUser: Types.user,
      ~channel: option(Types.channel),
      children,
    ) => {
  ...component,
  render: _ => {
    let channelId = channelPage.channelId;

    let topOrdinaryChannel = (channel: Types.channel) => {
      let joinButton = channelId =>
        E.React.showIf(
          Primary.Permissions.can(`JOIN_CHANNEL, channel.permissions),
          C.Channel.SimpleHeader.joinChannel(channelId),
        );

      let leaveButton = channelId =>
        E.React.showIf(
          Primary.Permissions.can(`LEAVE_CHANNEL, channel.permissions),
          C.Channel.SimpleHeader.leaveChannel(channelId),
        );

      <>
        <Div float=`left> {channelLink(channel)} </Div>
        <Div float=`right>
          {channel.myRole === Some(`NONE)
             ? joinButton(channel.id)
             : <>
                 {Foretold__Components__Channel.SimpleHeader.newMeasurable(
                    channel.id,
                  )}
                 {leaveButton(channel.id)}
               </>}
        </Div>
      </>;
    };

    let topGlobalChannel = channel => {
      <> <Div float=`left> {channelLink(channel)} </Div> </>;
    };

    let top = channel =>
      switch (channelId) {
      | "" => topGlobalChannel(channel)
      | _ => topOrdinaryChannel(channel)
      };

    let headers = () => {
      let topOption = Routing.ChannelPage.SubPage.toTab(channelPage.subPage);
      let secondLevel = channel =>
        ChannelTabs.make(loggedInUser, topOption, channel);

      switch (channel) {
      | Some(channel) =>
        <>
          <FC.GroupHeader> {top(channel)} </FC.GroupHeader>
          <FC.GroupHeader.SubHeader>
            {secondLevel(channel)}
          </FC.GroupHeader.SubHeader>
        </>
      | _ => <div />
      };
    };

    <FillWithSidebar channelId={Some(channelId)} loggedInUser>
      {headers()}
      <> {children[0]} </>
    </FillWithSidebar>;
  },
};