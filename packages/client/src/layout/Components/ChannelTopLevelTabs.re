open Utils;
module TabTypes = {
  type t =
    | Measurables
    | Members(ChannelMemberTabs.t)
    | Options(ChannelInfoTabs.t);

  let toS = (t: t) =>
    switch (t) {
    | Measurables => "M"
    | Members(_) => "Me"
    | Options(_) => "O"
    };

  let toUrl = (channelId: string, t: t): Context__Routing.Url.t =>
    switch (t) {
    | Measurables => ChannelShow(channelId)
    | Members(_) => ChannelMembers(channelId)
    | Options(_) => ChannelEdit(channelId)
    };

  let fromPage = (page: Context.Routing.Route.channelPage): option(t) =>
    switch (page.subPage) {
    | Settings => Some(Options(Edit))
    | NewSeries => Some(Options(NewSeries))
    | InviteNewMember => Some(Members(Invite))
    | Members => Some(Members(View))
    | NewMeasurable => Some(Measurables)
    | Measurables(_) => Some(Measurables)
    | Series(_) => Some(Measurables)
    };
};

module Component = {
  open TabTypes;
  module Styles = {
    open Css;
    let tab = isActive => {
      let stylee = [
        float(`left),
        fontSize(`em(1.1)),
        color(isActive ? `hex("2595ed") : `hex("848484")),
        padding2(~v=`em(0.5), ~h=`em(0.7)),
      ];
      style(
        isActive ?
          [borderBottom(`px(4), `solid, `hex("1a90ec")), ...stylee] :
          stylee,
      );
    };
  };

  let tab = (isActive, interalUrl, str) =>
    <C.Link className={Styles.tab(isActive)} linkType={Internal(interalUrl)}>
      {str |> ste}
    </C.Link>;

  let tabs = (o: t, channel: Context.Primary.Channel.t) =>
    <div>
      {tab(o == Measurables, toUrl(channel.id, Measurables), "Questions")}
      {
        tab(
          o == Members(View),
          toUrl(channel.id, Members(View)),
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
          tab(
            o == Options(Edit),
            toUrl(channel.id, Options(Edit)),
            "Settings",
          ),
        )
      }
    </div>;
};