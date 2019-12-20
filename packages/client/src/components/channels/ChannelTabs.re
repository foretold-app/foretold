open Utils;
open Routing;

// @todo: To make a component.
let tab = (isActive, interalUrl, str) =>
  <FC.Tab isActive onClick={LinkType.onClick(Internal(interalUrl))}>
    {str |> ste}
  </FC.Tab>;

// @todo: To make a component.
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
    {tab(
       tabSelected == Measurables,
       tabToInternalUrl(channel.id, Measurables),
       "Questions",
     )}
    {tab(
       tabSelected == Updates,
       tabToInternalUrl(channel.id, Updates),
       "Activity",
     )}
    {E.React2.showIf(
       channel.id != "home",
       tab(
         tabSelected == Members,
         tabToInternalUrl(channel.id, Members),
         (
           channel.membershipCount
           |> E.O.fmap(string_of_int)
           |> E.O.fmap(e => e ++ " ")
           |> E.O.default("")
         )
         ++ "Members",
       ),
     )}
    {tab(
       tabSelected == Notebooks,
       tabToInternalUrl(channel.id, Notebooks),
       (
         channel.notebooksCount
         |> E.O.fmap(string_of_int)
         |> E.O.fmap(e => e ++ " ")
         |> E.O.default("")
       )
       ++ "Notebooks",
     )}
    {E.React2.showIf(
       channel.id != "home",
       tab(
         tabSelected == Leaderboard,
         tabToInternalUrl(channel.id, Leaderboard),
         "Scores",
       ),
     )}
    {E.React2.showIf(
       channel.myRole === Some(`ADMIN),
       tab(
         tabSelected == Options,
         tabToInternalUrl(channel.id, Options),
         "Settings",
       ),
     )}
  </>;