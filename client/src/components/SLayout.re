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
      selector(
        "h1",
        [color(`hex("333")), fontSize(`em(2.0)), fontWeight(`bold)],
      ),
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