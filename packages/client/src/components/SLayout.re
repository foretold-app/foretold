open Utils;

type container = [ | `fixedWidth | `fluid | `none | `fluidLeft];

module Styles = {
  open Css;

  let width100 = style([width(`percent(100.))]);
  let headerText = style([color(`hex("486474"))]);
  let descriptionText = style([color(`hex("aaa"))]);

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

  let container = (container: container) =>
    switch (container) {
    | `fixedWidth => style([maxWidth(`px(1170)), margin(`auto)])
    | `fluid => style([paddingLeft(`em(2.0)), paddingRight(`em(2.0))])
    | `fluidLeft => style([paddingRight(`em(2.0))])
    | `none => style([])
    };

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
  [@react.component]
  let make = (~children=<Null />) =>
    <FC.PageCard.HeaderRow> children </FC.PageCard.HeaderRow>;
};

module TextDiv = {
  [@react.component]
  let make = (~text) =>
    <FC__PageCard.HeaderRow.Title>
      {text |> ste}
    </FC__PageCard.HeaderRow.Title>;
};

module ChannelBack = {
  [@react.component]
  let make = (~onClick, ()) =>
    <FC__Button onClick size=FC__Button.Small>
      <Antd_Tooltip title={Lang.backButtonTip |> Utils.ste} placement=`bottom>
        {"< Back" |> Utils.ste}
      </Antd_Tooltip>
    </FC__Button>;
};

module ChannelLink = {
  [@react.component]
  let make = (~channel: Types.channel) =>
    <Link
      linkType={Internal(ChannelShow(channel.id))}
      className=Styles.channelText>
      {switch (channel.id) {
       | "home" =>
         Primary.Channel.presentGlobal(
           ~symbolClassName=Primary.Channel.Styles.globe,
           (),
         )
       | _ => channel |> Primary.Channel.present
       }}
    </Link>;
};

module SeriesHead = {
  [@react.component]
  let make = (~seriesName) =>
    <div className=Styles.seriesText>
      <Icon icon="LAYERS" />
      {seriesName |> ste}
    </div>;
};

module Container = {
  [@react.component]
  let make = (~container=`fixedWidth, ~children) =>
    <div className={Styles.container(container)}> children </div>;
};

[@react.component]
let make =
    (~head=ReasonReact.null, ~container=`fixedWidth, ~children=<Null />) => {
  <FC.Base.Div
    className=Css.(
      style(
        [
          marginTop(`em(1.)),
          width(`percent(100.)),
          boxSizing(`borderBox),
        ]
        @ FC.Base.BaseStyles.fullWidthFloatLeft,
      )
    )>
    <Container container>
      <FC.PageCard>
        {head != ReasonReact.null
           ? <FC.PageCard.HeaderRow> head </FC.PageCard.HeaderRow> : head}
        <FC.PageCard.Body> children </FC.PageCard.Body>
      </FC.PageCard>
    </Container>
  </FC.Base.Div>;
};