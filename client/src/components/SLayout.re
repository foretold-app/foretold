open Utils;
open Style.Grid;
open Rationale.Function.Infix;

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
  let container = style([maxWidth(`px(1170)), margin(`auto)]);
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
  <Antd.Button onClick className=Styles.backHover>
    <Icon.Icon icon="ARROW_LEFT" />
    {"Back" |> ste}
  </Antd.Button>;

let channelink = (c: Context.Primary.Channel.t) =>
  <div className=Styles.headerText>
    {c |> Context.Primary.Channel.present}
  </div>;

let channelEditLink = (c: Context.Primary.Channel.t) =>
  <div
    className=Styles.headerText
    onClick={_ => Context.Routing.Url.push(ChannelEdit(c.id))}>
    {"edit" |> ste}
  </div>;

let seriesHead = (channel: Context.Primary.Channel.t, seriesName) =>
  <>
    <div className=Styles.seriesText>
      <Icon.Icon icon="LAYERS" />
      {seriesName |> ste}
    </div>
  </>;