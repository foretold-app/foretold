open Utils;
open Style.Grid;

module Styles = {
  open Css;
  let header =
    style([
      borderBottom(`px(1), `solid, `hex("eee")),
      paddingLeft(`px(10)),
      paddingBottom(`em(0.8)),
      paddingRight(`em(0.4)),
      paddingTop(`px(10)),
      float(`left),
      width(`percent(100.)),
    ]);
  let headerText =
    style([
      color(`hex("333")),
      fontSize(`em(2.0)),
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
  let backHover =
    style([
      color(`hex("8b949e")),
      background(`hex("e2e8ea")),
      fontSize(`em(1.3)),
      float(`left),
      padding(`px(6)),
      lineHeight(`px(0)),
      borderRadius(`percent(50.)),
      marginRight(`em(0.7)),
      marginTop(`em(0.3)),
      cursor(`pointer),
      selector(
        ":hover",
        [color(`hex("445b7d")), background(`hex("c2cbd4"))],
      ),
    ]);
  let foo =
    style([
      color(`hex("333")),
      fontSize(`em(2.0)),
      fontWeight(`bold),
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
    render: _ => <div className=Styles.mainSection> ...children </div>,
  };
};

let channelBack =
    (
      ~channelName: string,
      ~onClick=_ => Urls.pushToLink(ChannelShow(channelName)),
      (),
    ) =>
  <div className=Styles.backHover onClick>
    <Icon.Icon icon="ARROW_LEFT" />
  </div>;

let channelLink = (c: string) =>
  <div className=Styles.headerText> {"#" ++ c |> ste} </div>;

let seriesHead = (channelName, seriesName) =>
  <div>
    {channelBack(~channelName, ())}
    <div className=Styles.headerText> {"#" ++ channelName |> ste} </div>
    <div className=Styles.dash> <Icon.Icon icon="THIN_RIGHT" /> </div>
    <div className=Styles.seriesText>
      <Icon.Icon icon="LAYERS" />
      {seriesName |> ste}
    </div>
  </div>;

let button = channel =>
  <Div float=`right>
    <Antd.Button onClick={_ => Urls.pushToLink(MeasurableNew(channel))}>
      {"New Measurable" |> ste}
    </Antd.Button>
  </Div>;

let channelHeader = channel =>
  <div className=Styles.header>
    {
      channel
      |> E.O.fmap(c =>
           <div>
             <h1>
               <a href={Urls.mapLinkToUrl(ChannelShow(c))}>
                 {"#" ++ c |> ste}
               </a>
             </h1>
             {button(c)}
           </div>
         )
      |> E.O.React.defaultNull
    }
  </div>;