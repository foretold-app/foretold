open Utils;

module Styles = {
  open Css;
  let header =
    style([
      paddingLeft(`em(1.)),
      paddingBottom(`em(0.8)),
      paddingRight(`em(1.)),
      paddingTop(`em(1.5)),
      float(`left),
      width(`percent(100.)),
    ]);
  let width100 = style([width(`percent(100.))]);
  let headerText =
    style([
      color(`hex("486474")),
      fontSize(`em(1.8)),
      fontWeight(`bold),
      float(`left),
    ]);
  let seriesText =
    style([
      color(`hex("3d547b")),
      fontSize(`em(2.0)),
      float(`left),
      fontWeight(`medium),
    ]);
  let channelText =
    style([color(`hex("242424")), fontSize(`em(1.4)), float(`left)]);
  let container = style([maxWidth(`px(1170)), margin(`auto)]);
  let header1outer =
    style([
      width(`percent(100.0)),
      float(`left),
      backgroundColor(`hex("fff")),
      borderBottom(`px(1), `solid, `hex("e6e9f0")),
    ]);
  let header1inner =
    style([
      width(`percent(100.0)),
      float(`left),
      fontWeight(`num(600)),
      padding2(~v=`em(0.7), ~h=`em(1.0)),
    ]);
  let header2outer =
    style([
      width(`percent(100.0)),
      float(`left),
      backgroundColor(`hex("fbfcfd")),
      borderBottom(`px(1), `solid, `hex("e6e9f0")),
    ]);
  let header2inner =
    style([
      width(`percent(100.0)),
      float(`left),
      paddingLeft(`em(1.)),
      paddingRight(`em(1.)),
      paddingTop(`em(0.5)),
    ]);
  let header2 = style([background(`hex("f0f"))]);
  let backHover = style([fontSize(`em(1.3))]);
  let foo =
    style([
      color(`hex("333")),
      fontSize(`em(2.0)),
      fontWeight(`bold),
      float(`left),
    ]);
  let largeCardOuter = style([padding(`em(0.3))]);
  let largeCardInner =
    style([
      background(`hex("fff")),
      border(`px(1), `solid, `hex("e1e5f0")),
      borderRadius(`px(5)),
      padding(`em(1.)),
      width(`percent(100.)),
      float(`left),
    ]);
  let mainSection =
    style([
      float(`left),
      width(`percent(100.)),
      paddingLeft(`px(10)),
      paddingRight(`px(10)),
      paddingTop(`px(10)),
    ]);
  let dash =
    style([
      float(`left),
      marginTop(`em(0.2)),
      marginRight(`em(0.3)),
      marginLeft(`em(0.3)),
      fontSize(`em(1.5)),
      color(`hex("ccc")),
    ]);
};

module LargeCard = {
  let component = ReasonReact.statelessComponent("LargeCard");
  let make = children => {
    ...component,
    render: _ =>
      <div className=Styles.largeCardOuter>
        <div className=Styles.largeCardInner> ...children </div>
      </div>,
  };
};
module Header = {
  let component = ReasonReact.statelessComponent("Header");
  let textDiv = text => <div className=Styles.headerText> {text |> ste} </div>;

  let make = children => {
    ...component,
    render: _ => <div className=Styles.header> ...children </div>,
  };
};

module MainSection = {
  let component = ReasonReact.statelessComponent("MainSection");
  let make = children => {
    ...component,
    render: _ =>
      <div className=Styles.mainSection>
        <LargeCard> ...children </LargeCard>
      </div>,
  };
};

module LayoutConfig = {
  type t = {
    head: ReasonReact.reactElement,
    body: ReasonReact.reactElement,
  };
  let make = (~head, ~body) => {head, body};
};

module FullPage = {
  let component = ReasonReact.statelessComponent("Page");
  let make = ({head, body}: LayoutConfig.t) => {
    ...component,
    render: _ =>
      <div className=Styles.container>
        <Header> head </Header>
        <MainSection> body </MainSection>
      </div>,
  };
  let makeWithEl = (t: LayoutConfig.t) => t |> make |> E.React.el;
};

let channelBack = (~onClick, ()) =>
  <FC__Button onClick> {"< Back" |> ReasonReact.string} </FC__Button>;

let channelink = (c: Context.Primary.Channel.t) =>
  <Foretold__Components__Link
    linkType={Internal(ChannelShow(c.id))} className=Styles.channelText>
    {c |> Context.Primary.Channel.present}
  </Foretold__Components__Link>;

let channelEditLink = (c: Context.Primary.Channel.t) =>
  <Foretold__Components__Link
    linkType={Internal(ChannelEdit(c.id))} className=Styles.headerText>
    {"edit" |> ste}
  </Foretold__Components__Link>;

let seriesHead = (channel: Context.Primary.Channel.t, seriesName) =>
  <>
    <div className=Styles.seriesText>
      <Icon.Icon icon="LAYERS" />
      {seriesName |> ste}
    </div>
  </>;

module SidebarSection = {
  module Styles_ = {
    open Css;
    let containerOuter = style([paddingTop(`px(10))]);
    let containerInner =
      style([
        background(`hex("fff")),
        borderRadius(`px(5)),
        width(`percent(100.)),
        border(`px(1), `solid, `hex("e8eaef")),
        float(`left),
      ]);
    let header =
      style([
        float(`left),
        fontSize(`em(1.1)),
        width(`percent(100.)),
        color(`hex("242424")),
        fontWeight(`num(600)),
        padding(`em(1.)),
        padding2(~v=`em(0.5), ~h=`em(0.7)),
        borderBottom(`px(1), `solid, `hex("e6e9f0")),
      ]);
    let body =
      style([
        float(`left),
        padding(`em(1.)),
        fontSize(`em(1.1)),
        padding2(~v=`em(0.5), ~h=`em(0.7)),
        color(`hex("858585")),
      ]);
  };

  module Container = {
    let component = ReasonReact.statelessComponent("Container");
    let make = children => {
      ...component,
      render: _ =>
        <div className=Styles_.containerOuter>
          <div className=Styles_.containerInner> ...children </div>
        </div>,
    };
  };

  module Header = {
    let component = ReasonReact.statelessComponent("Container");
    let make = children => {
      ...component,
      render: _ => <div className=Styles_.header> ...children </div>,
    };
  };

  module Body = {
    let component = ReasonReact.statelessComponent("Body");
    let make = children => {
      ...component,
      render: _ => <div className=Styles_.body> ...children </div>,
    };
  };
};