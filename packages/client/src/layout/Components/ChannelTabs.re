open Utils;
open Context.Routing;

let tab = (isActive, interalUrl, str) =>
  <FC.Tab isActive onClick={C.Link.LinkType.onClick(Internal(interalUrl))}>
    {str |> ste}
  </FC.Tab>;

let tabToInternalUrl = (channelId, o: ChannelPage.tab): Url.t => {
  let channelPage: ChannelPage.t = {
    channelId,
    subPage: ChannelPage.SubPage.fromTab(o),
  };
  channelPage |> Url.fromChannelPage;
};

let make =
    (o: Context.Routing.ChannelPage.tab, channel: Context.Primary.Channel.t) =>
  <>
    {
      tab(
        o == Measurables,
        tabToInternalUrl(channel.id, Measurables),
        "Questions",
      )
    }
    {
      tab(
        o == Members,
        tabToInternalUrl(channel.id, Members),
        (
          channel.membershipCount
          |> E.O.fmap(string_of_int)
          |> E.O.fmap(e => e ++ " ")
          |> E.O.default("")
        )
        ++ "Members",
      )
    }
    {
      E.React.showIf(
        channel.myRole === Some(`ADMIN),
        tab(o == Options, tabToInternalUrl(channel.id, Options), "Settings"),
      )
    }
  </>;