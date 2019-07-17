open SLayout;
open Style.Grid;

let component = ReasonReact.statelessComponent("Channel Layout Page");

let make =
    (
      channelPage: Routing.ChannelPage.t,
      loggedInUser: Primary.User.t,
      channel: option(Primary.Channel.t),
      {head, body}: LayoutConfig.t,
    ) => {
  ...component,
  render: _ => {
    let channelId = channelPage.channelId;

    let topOrdinaryChannel = channel => {
      let joinButton = channelId =>
        C.Channel.SimpleHeader.joinChannel(channelId);

      let leaveButton = channelId =>
        C.Channel.SimpleHeader.leaveChannel(channelId);

      <>
        <Div float=`left> {channelink(channel)} </Div>
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
      <> <Div float=`left> {channelink(channel)} </Div> </>;
    };

    let top = channel =>
      switch (channelId) {
      | "" => topGlobalChannel(channel)
      | _ => topOrdinaryChannel(channel)
      };

    let headers = () => {
      let topOption = Routing.ChannelPage.SubPage.toTab(channelPage.subPage);
      let secondLevel = channel => ChannelTabs.make(topOption, channel);

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

    <Layout__Component__FillWithSidebar
      channelId={Some(channelId)} loggedInUser>
      {headers()}
      <div className=Styles.container>
        <Div flexDirection=`row styles=[SLayout.Styles.width100]>
          <Div
            styles=[
              Css.(
                style([
                  marginTop(`em(1.0)),
                  paddingRight(`em(1.0)),
                  paddingLeft(`em(1.0)),
                ])
              ),
            ]
            flex=3>
            <FC.PageCard>
              <FC.PageCard.HeaderRow> head </FC.PageCard.HeaderRow>
              body
            </FC.PageCard>
          </Div>
        </Div>
      </div>
    </Layout__Component__FillWithSidebar>;
  },
};

let makeWithEl =
    (
      channelPage: Routing.ChannelPage.t,
      loggedInUser,
      channel: option(Primary.Channel.t),
      layout: LayoutConfig.t,
    ) =>
  make(channelPage, loggedInUser, channel, layout) |> ReasonReact.element;