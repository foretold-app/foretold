open SLayout;
open Utils;
open Style.Grid;
open Rationale.Function.Infix;

let component = ReasonReact.statelessComponent("Channel Layout Page");

let make =
    (
      channelPage: Context.Routing.ChannelPage.t,
      loggedInUser: Context.Primary.User.t,
      {head, body}: LayoutConfig.t,
    ) => {
  ...component,
  render: _ => {
    let channelId = channelPage.channelId;
    let topOption =
      Context.Routing.ChannelPage.SubPage.toTab(channelPage.subPage);

    let loadChannel =
      Foretold__GraphQL.Queries.Channel.component2(~id=channelId);

    let joinButton = channelId =>
      C.Channel.SimpleHeader.joinChannel(channelId);
    let leaveButton = channelId =>
      C.Channel.SimpleHeader.leaveChannel(channelId);

    let top = channel =>
      <>
        <Div float=`left> {channelink(channel)} </Div>
        <Div float=`right>
          {
            channel.myRole === Some(`NONE) ?
              joinButton(channel.id) :
              <>
                {
                  Foretold__Components__Channel.SimpleHeader.newMeasurable(
                    channel.id,
                  )
                }
                {leaveButton(channel.id)}
              </>
          }
        </Div>
      </>;

    let secondLevel = channel => ChannelTabs.make(topOption, channel);

    let headers =
      loadChannel(
        E.HttpResponse.fmap((channel: Context.Primary.Channel.t) =>
          <>
            <FC.GroupHeader> {top(channel)} </FC.GroupHeader>
            <FC.GroupHeader.SubHeader>
              {secondLevel(channel)}
            </FC.GroupHeader.SubHeader>
          </>
        )
        ||> E.HttpResponse.withReactDefaults,
      );

    <Layout__Component__FillWithSidebar
      channelId={Some(channelId)} loggedInUser>
      headers
      <div className=Styles.container>
        <Div flexDirection=`row styles=[SLayout.Styles.width100]>
          <Div
            styles=[
              Css.(style([marginTop(`em(1.0)), marginRight(`em(2.0))])),
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
      channelPage: Context.Routing.ChannelPage.t,
      loggedInUser,
      t: LayoutConfig.t,
    ) =>
  make(channelPage, loggedInUser, t) |> E.React.el;