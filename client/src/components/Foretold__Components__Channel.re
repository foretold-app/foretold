open Utils;
open Style.Grid;

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

  let button = channelId =>
    <Div float=`right>
      <Antd.Button
        onClick={_ => DataModel.Url.push(MeasurableNew(channelId))}>
        {"New Measurable" |> ste}
      </Antd.Button>
    </Div>;
};