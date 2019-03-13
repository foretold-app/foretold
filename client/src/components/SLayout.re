open Utils;
open Style.Grid;

let component = ReasonReact.statelessComponent("Measurable");

module Styles = {
  open Css;
  let header =
    style([
      borderBottom(`px(1), `solid, `hex("eee")),
      paddingLeft(`px(10)),
      paddingBottom(`em(0.8)),
      paddingRight(`em(0.4)),
      paddingTop(`px(10)),
    ]);
  let headerText =
    style([color(`hex("333")), fontSize(`em(2.0)), fontWeight(`bold)]);
  let mainSection =
    style([
      paddingLeft(`px(10)),
      paddingRight(`px(10)),
      paddingTop(`px(10)),
    ]);
};

let make = (~id: string, ~loggedInUser, _children) => {
  ...component,
  render: _self => <MeasurableShow__Component id loggedInUser />,
};

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
             <a
               href={Urls.mapLinkToUrl(ChannelShow(c))}
               className=Styles.headerText>
               {"#" ++ c |> ste}
             </a>
             {button(c)}
           </div>
         )
      |> E.O.React.defaultNull
    }
  </div>;

let indexChannelHeader =
    (channel: option(string), page: int, onForward, onBackward, isAtEnd) =>
  <div className=Styles.header>
    {
      channel
      |> E.O.fmap(c =>
           <div>
             <a
               href={Urls.mapLinkToUrl(ChannelShow(c))}
               className=Styles.headerText>
               {"#" ++ c |> ste}
             </a>
             <Antd.Button onClick={_ => onBackward()} disabled={page == 0}>
               <Icon.Icon icon="ARROW_LEFT" />
             </Antd.Button>
             <Antd.Button onClick={_ => onForward()} disabled=isAtEnd>
               <Icon.Icon icon="ARROW_RIGHT" />
             </Antd.Button>
             {button(c)}
           </div>
         )
      |> E.O.React.defaultNull
    }
  </div>;