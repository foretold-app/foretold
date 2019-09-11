open SLayout;
open Style.Grid;

let component = ReasonReact.statelessComponent("Channel");
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
          SimpleHeader.joinChannel(channelId),
        );

      let leaveButton = channelId =>
        E.React.showIf(
          Primary.Permissions.can(`LEAVE_CHANNEL, channel.permissions),
          SimpleHeader.leaveChannel(channelId),
        );

      <>
        <Div float=`left> {channelLink(channel)} </Div>
        <Div float=`right>
          {channel.myRole === Some(`NONE)
             ? joinButton(channel.id)
             : <>
                 {SimpleHeader.newMeasurable(channel.id)}
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
          <Fc.FC.GroupHeader> {top(channel)} </Fc.FC.GroupHeader>
          <Fc.FC.GroupHeader.SubHeader>
            {secondLevel(channel)}
          </Fc.FC.GroupHeader.SubHeader>
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