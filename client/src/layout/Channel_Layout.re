open SLayout;
open Utils;
open Style.Grid;
open Rationale.Function.Infix;

/* module MemberTabs = {
     type memberOption =
       | View
       | Invite
       | Edit;
   }

   module TopTabs = {

     type t =
       | Measurables
       | Members(MemberTabs.t)
       | Options(InfoTabs.t);

     let toS = (t: t) =>
       switch (t) {
       | Measurables => "M"
       | Members(_) => "Me"
       | Options(_) => "O"
       };
   } */

module Config = {
  type memberOption =
    | View
    | Invite
    | Edit;

  let memberOptionToS = (t: memberOption) =>
    switch (t) {
    | View => "V"
    | Invite => "I"
    | Edit => "Edit"
    };

  type editOptions =
    | NewSeries
    | Edit;

  let editOptionToS = (t: editOptions) =>
    switch (t) {
    | NewSeries => "N"
    | Edit => "E"
    };

  type topOption =
    | Measurables
    | Members(memberOption)
    | Options(editOptions);

  let topOptionToS = (t: topOption) =>
    switch (t) {
    | Measurables => "M"
    | Members(_) => "Me"
    | Options(_) => "O"
    };

  type t = {
    head: ReasonReact.reactElement,
    body: ReasonReact.reactElement,
  };
};

let component = ReasonReact.statelessComponent("Page");

let button = (value, str) =>
  <Antd_Radio_Button value> {str |> ste} </Antd_Radio_Button>;

let tabs = (o: Config.topOption, channel: Context.Primary.Channel.t) =>
  <Antd.Radio.Group
    defaultValue="" value={o |> Config.topOptionToS} onChange={e => ()}>
    {button(Config.Measurables |> Config.topOptionToS, "Measurables")}
    {
      button(
        Config.Members(View) |> Config.topOptionToS,
        (
          channel.membershipCount
          |> E.O.fmap(string_of_int)
          |> E.O.fmap(e => e ++ " ")
          |> E.O.default("")
        )
        ++ "Members",
      )
    }
    {button(Config.Options(Edit) |> Config.topOptionToS, "Info")}
  </Antd.Radio.Group>;

let memberTabs = (o: Config.memberOption) =>
  <Antd.Radio.Group
    defaultValue="" value={o |> Config.memberOptionToS} onChange={e => ()}>
    {button(Config.Measurables |> Config.topOptionToS, "Members")}
    {button(Config.Measurables |> Config.topOptionToS, "Add Members")}
  </Antd.Radio.Group>;

let editTabs = (o: Config.editOptions) =>
  <Antd.Radio.Group
    defaultValue="foo" value={o |> Config.editOptionToS} onChange={e => ()}>
    {button(Config.Edit |> Config.editOptionToS, "Edit")}
    {button(Config.NewSeries |> Config.editOptionToS, "New Series")}
  </Antd.Radio.Group>;

let make =
    (
      channelId: string,
      loggedInUser: Context.Primary.User.t,
      topOption: option(Config.topOption),
      {head, body}: LayoutConfig.t,
    ) => {
  ...component,
  render: _ => {
    let topOption =
      topOption
      |> E.O.toExn(
           "The top option should always be SOME in Channel_Layout.make",
         );

    let loadChannel =
      Foretold__GraphQL.Queries.Channel.component2(~id=channelId);

    let top =
      loadChannel(
        E.HttpResponse.fmap((result: Context.Primary.Channel.t) =>
          <>
            <Div float=`left> {channelink(result)} </Div>
            <Div float=`right> {tabs(topOption, result)} </Div>
          </>
        )
        ||> E.HttpResponse.withReactDefaults,
      );

    <Layout__Component__FillWithSidebar
      channelId={Some(channelId)} loggedInUser>
      <MainSection> top </MainSection>
      {
        switch (topOption) {
        | Members(r) => <MainSection> {memberTabs(r)} </MainSection>
        | Options(r) => <MainSection> {editTabs(r)} </MainSection>
        | _ => E.React.null
        }
      }
      <MainSection> head </MainSection>
      <MainSection> body </MainSection>
    </Layout__Component__FillWithSidebar>;
  },
};

let makeWithEl = (channelId, loggedInUser, topOption, t: LayoutConfig.t) =>
  make(channelId, loggedInUser, topOption, t) |> E.React.el;

let topOption = (page: Context.Routing.Route.t): option(Config.topOption) =>
  switch (page) {
  | ChannelEdit(_) => Some(Options(Edit))
  | SeriesNew(_) => Some(Options(NewSeries))
  | ChannelInvite(_) => Some(Members(Invite))
  | ChannelMembers(_) => Some(Members(View))
  | MeasurableNew(_) => Some(Measurables)
  | ChannelShow(_) => Some(Measurables)
  | Series(_, _) => Some(Options(Edit))
  | _ => None
  };

let makeWithPage =
    (page: Context.Routing.Route.t, loggedInUser): ReasonReact.reactElement => {
  let topOption = topOption(page);

  let layout = (channelId, fn) =>
    makeWithEl(channelId, loggedInUser, topOption) |> fn |> E.React.makeToEl;

  switch (page) {
  | ChannelShow(channelId) =>
    MeasurableIndex.make(
      ~channelId,
      ~loggedInUser,
      ~itemsPerPage=20,
      ~layout=_,
    )
    |> layout(channelId)
  | Series(channelId, id) =>
    SeriesShow.make(~id, ~channelId, ~loggedInUser, ~layout=_)
    |> layout(channelId)
  | MeasurableNew(channelId) =>
    MeasurableNew.make(~channelId, ~layout=_) |> layout(channelId)
  | ChannelMembers(channelId) =>
    ChannelMembers.make(~channelId, ~loggedInUser, ~layout=_)
    |> layout(channelId)
  | ChannelInvite(channelId) =>
    ChannelInvite.make(~channelId, ~loggedInUser, ~layout=_)
    |> layout(channelId)
  | ChannelEdit(channelId) =>
    ChannelEdit.make(~channelId, ~layout=_) |> layout(channelId)
  | SeriesNew(channelId) =>
    SeriesNew.make(~channelId, ~loggedInUser, ~layout=_) |> layout(channelId)
  | _ => E.React.null
  };
};