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
        <Div float=`left flexDirection=`column>
          <Div flex={`num(1.0)}> {channelLink(channel)} </Div>
          {channel.description
           |> E.O.React.fmapOrNull(source =>
                <Div
                  flex={`num(1.0)}
                  styles=[Css.(style([marginTop(`em(0.5))]))]>
                  <ReactMarkdown.Markdown source />
                </Div>
              )}
        </Div>
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
      <Div float=`left flexDirection=`column>
        <Div flex={`num(1.0)}> {channelLink(channel)} </Div>
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
                          color(FC.Base.Colors.textDark),
                        ],
                      ),
                    ])
                  ),
                ]>
                <ReactMarkdown.Markdown source />
              </Div>
            )}
      </Div>;
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
      {children |> ReasonReact.array}
    </FillWithSidebar>;
  },
};