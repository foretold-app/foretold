open Utils;
open Style.Grid;

let simpleLink = c =>
  <a href={DataModel.Channel.showUrl(c)}>
    {DataModel.Channel.present(c)}
  </a>;

module SimpleHeader = {
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
  };

  let button = channel =>
    <Div float=`right>
      <Antd.Button onClick={_ => Urls.pushToLink(MeasurableNew(channel))}>
        {"New Measurable" |> ste}
      </Antd.Button>
    </Div>;

  let component = ReasonReact.statelessComponent("ChannelSimpleHeader");

  let make = (~channel: option(string), _children) => {
    ...component,
    render: _ =>
      <div className=Styles.header>
        {
          channel
          |> E.O.fmap(c => <> <h1> {simpleLink(c)} </h1> {button(c)} </>)
          |> E.O.React.defaultNull
        }
      </div>,
  };
};