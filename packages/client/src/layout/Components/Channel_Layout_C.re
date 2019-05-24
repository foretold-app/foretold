open SLayout;
open Utils;
open Style.Grid;
open Rationale.Function.Infix;

module Config = {
  type t = {
    head: ReasonReact.reactElement,
    body: ReasonReact.reactElement,
  };
};

let component = ReasonReact.statelessComponent("Page");

let joinButton = channelId => C.Channel.SimpleHeader.joinChannel(channelId);

let make =
    (
      channelPage: Context.Routing.Route.channelPage,
      loggedInUser: Context.Primary.User.t,
      {head, body}: LayoutConfig.t,
    ) => {
  ...component,
  render: _ => {
    let channelId = channelPage.channelId;
    let topOption =
      ChannelTopLevelTabs.TabTypes.fromPage(channelPage)
      |> E.O.toExn(
           "The top option should always be SOME in Channel_Layout.make",
         );

    let loadChannel =
      Foretold__GraphQL.Queries.Channel.component2(~id=channelId);

    let top =
      loadChannel(
        E.HttpResponse.fmap((channel: Context.Primary.Channel.t) =>
          <> {channelink(channel)} </>
        )
        ||> E.HttpResponse.withReactDefaults,
      );

    let leaveButton = channelId =>
      C.Channel.SimpleHeader.leaveChannel(channelId);
    let sidebar1 =
      loadChannel(
        E.HttpResponse.fmap((channel: Context.Primary.Channel.t) =>
          <SLayout.SidebarSection.Container>
            <SLayout.SidebarSection.Header>
              {channel |> Context.Primary.Channel.present}
            </SLayout.SidebarSection.Header>
            <SLayout.SidebarSection.Body>
              {channel.description |> E.O.default("") |> ste}
              {
                Foretold__Components__Channel.SimpleHeader.newMeasurable(
                  channel.id,
                )
              }
              {
                channel.myRole === Some(`NONE) ?
                  joinButton(channel.id) : leaveButton(channel.id)
              }
            </SLayout.SidebarSection.Body>
          </SLayout.SidebarSection.Container>
        )
        ||> E.HttpResponse.withReactDefaults,
      );

    let secondLevel =
      loadChannel(
        E.HttpResponse.fmap((channel: Context.Primary.Channel.t) =>
          <Div>
            {ChannelTopLevelTabs.Component.tabs(topOption, channel)}
          </Div>
        )
        ||> E.HttpResponse.withReactDefaults,
      );

    let bottomHeader =
      loadChannel(
        E.HttpResponse.fmap((channel: Context.Primary.Channel.t) =>
          switch (topOption) {
          | Members(r) =>
            <Div float=`right>
              {ChannelMemberTabs.component(loggedInUser.agent, r, channel)}
            </Div>
          | Options(r) =>
            <Div float=`right> {ChannelInfoTabs.component(r, channel)} </Div>
          | _ => E.React.null
          }
        )
        ||> E.HttpResponse.withReactDefaults,
      );

    <Layout__Component__FillWithSidebar
      channelId={Some(channelId)} loggedInUser>
      <div className=Styles.header1outer>
        <div className=Styles.container>
          <div className=Styles.header1inner> top </div>
        </div>
      </div>
      <div className=Styles.header2outer>
        <div className=Styles.container>
          <div className=Styles.header2inner> secondLevel </div>
        </div>
      </div>
      <div className=Styles.container>
        <Div flexDirection=`row styles=[SLayout.Styles.width100]>
          <Div flex=3>
            <MainSection>
              <Div flexDirection=`column>
                <Div
                  flex=1 styles=[Css.style([Css.marginBottom(`em(1.))])]>
                  bottomHeader
                  head
                </Div>
                <Div flex=1> body </Div>
              </Div>
            </MainSection>
          </Div>
          <Div flex=1> sidebar1 </Div>
        </Div>
      </div>
    </Layout__Component__FillWithSidebar>;
  },
};

let makeWithEl =
    (
      channelPage: Context.Routing.Route.channelPage,
      loggedInUser,
      t: LayoutConfig.t,
    ) =>
  make(channelPage, loggedInUser, t) |> E.React.el;