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

    let joinButton = channelId =>
      C.Channel.SimpleHeader.joinChannel(channelId);
    let leaveButton = channelId =>
      C.Channel.SimpleHeader.leaveChannel(channelId);

    let top =
      loadChannel(
        E.HttpResponse.fmap((channel: Context.Primary.Channel.t) =>
          <>
            <Div float=`left> {channelink(channel)} </Div>
            <Div float=`right>
              {
                Foretold__Components__Channel.SimpleHeader.newMeasurable(
                  channel.id,
                )
              }
              {
                channel.myRole === Some(`NONE) ?
                  joinButton(channel.id) : leaveButton(channel.id)
              }
            </Div>
          </>
        )
        ||> E.HttpResponse.withReactDefaults,
      );

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
          ChannelTopLevelTabs.Component.tabs(topOption, channel)
        )
        ||> E.HttpResponse.withReactDefaults,
      );

    <Layout__Component__FillWithSidebar
      channelId={Some(channelId)} loggedInUser>
      <FC.GroupHeader> top </FC.GroupHeader>
      <FC.GroupHeader.SubHeader> secondLevel </FC.GroupHeader.SubHeader>
      <div className=Styles.container>
        <Div flexDirection=`row styles=[SLayout.Styles.width100]>
          <Div
            styles=[
              Css.(style([marginTop(`em(1.0)), marginRight(`em(2.0))])),
            ]
            flex=3>
            <FC.PageCard> {FC.PageCard.header(head)} body </FC.PageCard>
          </Div>
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