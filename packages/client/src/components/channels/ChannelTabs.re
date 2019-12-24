open Utils;
open Routing;

module Tab = {
  [@react.component]
  let make = (~isActive, ~interalUrl, ~str) =>
    <FC.Tab isActive onClick={LinkType.onClick(Internal(interalUrl))}>
      {str |> ste}
    </FC.Tab>;
};

let tabToInternalUrl = (channelId, tabSelected: ChannelPage.tab): Url.t => {
  let channelPage: ChannelPage.t = {
    channelId,
    subPage: ChannelPage.SubPage.fromTab(tabSelected),
  };
  channelPage |> Url.fromChannelPage;
};

[@react.component]
let make = (~tabSelected: Routing.ChannelPage.tab, ~channel: Types.channel) =>
  <>
    <Tab
      isActive={tabSelected == Measurables}
      interalUrl={tabToInternalUrl(channel.id, Measurables)}
      str="Questions"
    />
    <Tab
      isActive={tabSelected == Updates}
      interalUrl={tabToInternalUrl(channel.id, Updates)}
      str="Activity"
    />
    {E.React2.showIf(
       channel.id != "home",
       <Tab
         isActive={tabSelected == Members}
         interalUrl={tabToInternalUrl(channel.id, Members)}
         str={
           (
             channel.membershipCount
             |> E.O.fmap(string_of_int)
             |> E.O.fmap(e => e ++ " ")
             |> E.O.default("")
           )
           ++ "Members"
         }
       />,
     )}
    <Tab
      isActive={tabSelected == Notebooks}
      interalUrl={tabToInternalUrl(channel.id, Notebooks)}
      str={
        (
          channel.notebooksCount
          |> E.O.fmap(string_of_int)
          |> E.O.fmap(e => e ++ " ")
          |> E.O.default("")
        )
        ++ "Notebooks"
      }
    />
    {E.React2.showIf(
       channel.id != "home",
       <Tab
         isActive={tabSelected == Leaderboard}
         interalUrl={tabToInternalUrl(channel.id, Leaderboard)}
         str="Scores"
       />,
     )}
    {E.React2.showIf(
       channel.myRole === Some(`ADMIN),
       <Tab
         isActive={tabSelected == Options}
         interalUrl={tabToInternalUrl(channel.id, Options)}
         str="Settings"
       />,
     )}
  </>;