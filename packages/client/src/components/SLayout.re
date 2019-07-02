open Utils;

module Styles = {
  open Css;

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
    style([
      color(`hex("2c436e")),
      fontSize(`em(1.15)),
      fontWeight(`num(600)),
      marginTop(`px(3)),
      float(`left),
    ]);

  let container = style([maxWidth(`px(1170)), margin(`auto)]);

  let backHover = style([fontSize(`em(1.3))]);

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

module Header = {
  let component = ReasonReact.statelessComponent("Header");
  let textDiv = text =>
    <FC.PageCard.HeaderRow.Title> {text |> ste} </FC.PageCard.HeaderRow.Title>;

  let make = children => {
    ...component,
    render: _ => <FC.PageCard.HeaderRow> ...children </FC.PageCard.HeaderRow>,
  };
};

module Spin = {
  module Styles = {
    open Css;
    let centerBlock = style([textAlign(`center), padding(`em(2.))]);
    let clear = style([clear(`both)]);
  };

  let component = ReasonReact.statelessComponent("Spin");

  let make = _children => {
    ...component,
    render: _ =>
      <>
        <div className=Styles.clear />
        <div className=Styles.centerBlock>
          <Antd.Spin tip="Loading..." />
        </div>
      </>,
  };
};

module Error = {
  module Styles = {
    open Css;
    let centerBlock = style([textAlign(`center), padding(`em(2.))]);
    let clear = style([clear(`both)]);
  };

  let component = ReasonReact.statelessComponent("Error");

  let make = (~e, _children) => {
    ...component,
    render: _ => <> <div> {"Error: " ++ e |> Utils.ste} </div> </>,
  };
};

module NothingToShow = {
  module Styles = {
    open Css;
    let centerBlock = style([textAlign(`center), margin(`em(2.))]);
    let clear = style([clear(`both)]);
  };

  let component = ReasonReact.statelessComponent("NothingToShow");

  let make = _children => {
    ...component,
    render: _ =>
      <>
        <div className=Styles.clear />
        <div className=Styles.centerBlock>
          {"Nothing to show" |> Utils.ste}
        </div>
      </>,
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
      <FC.Base.Div
        className=Css.(
          style(
            [
              marginTop(`em(2.0)),
              marginTop(`em(2.)),
              width(`percent(100.)),
              paddingLeft(`em(2.)),
              paddingRight(`em(2.)),
              boxSizing(`borderBox),
            ]
            @ FC.Base.BaseStyles.fullWidthFloatLeft,
          )
        )>
        <div className=Styles.container>
          <FC.PageCard>
            <FC.PageCard.HeaderRow> head </FC.PageCard.HeaderRow>
            <FC.PageCard.Body> body </FC.PageCard.Body>
          </FC.PageCard>
        </div>
      </FC.Base.Div>,
  };
  let makeWithEl = (t: LayoutConfig.t) => t |> make |> E.React.el;
};

let channelBack = (~onClick, ()) =>
  <FC__Button onClick> {"< Back" |> ReasonReact.string} </FC__Button>;

let channelink = (c: Primary.Channel.t) =>
  <Foretold__Components__Link
    linkType={Internal(ChannelShow(c.id))} className=Styles.channelText>
    {c |> Primary.Channel.present}
  </Foretold__Components__Link>;

let channelEditLink = (c: Primary.Channel.t) =>
  <Foretold__Components__Link
    linkType={Internal(ChannelEdit(c.id))} className=Styles.headerText>
    {"edit" |> ste}
  </Foretold__Components__Link>;

let seriesHead = (channel: Primary.Channel.t, seriesName) =>
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