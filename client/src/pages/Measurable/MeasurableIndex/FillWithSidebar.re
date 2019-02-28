open Utils;
open Rationale;
open Queries;

module Styles = {
  open Css;
  let outer =
    style([display(`flex), flexDirection(`row), height(`percent(100.0))]);
  let left =
    style([
      flex(1),
      backgroundColor(`hex("2c436e")),
      minWidth(`px(200)),
    ]);
  let right = style([flex(7), width(`percent(100.0))]);
  let rightTop =
    style([
      borderBottom(`px(1), `solid, `hex("eee")),
      paddingLeft(`px(10)),
      paddingBottom(`em(0.8)),
      paddingRight(`em(0.4)),
      paddingTop(`px(10)),
    ]);
  let rightBottom =
    style([
      paddingLeft(`px(10)),
      paddingRight(`px(10)),
      paddingTop(`px(10)),
    ]);
  let header =
    style([color(`hex("333")), fontSize(`em(2.0)), fontWeight(`bold)]);
  let newButton =
    style([
      color(`hex("fff")),
      borderRadius(`px(4)),
      padding2(~v=`px(1), ~h=`em(0.3)),
      Css.cursor(`pointer),
      float(`right),
      fontSize(`em(1.2)),
      marginTop(`em(0.5)),
      backgroundColor(hex("1890ff")),
      selector(
        ":hover",
        [backgroundColor(hex("60b2ff")), color(`hex("fff"))],
      ),
    ]);
};

let component = ReasonReact.statelessComponent("MeOverlay");
let button = channel =>
  <a href={"/c/" ++ channel ++ "/new"} className=Styles.newButton>
    {"New Measurable" |> ste}
  </a>;

let make = (~channel, children) => {
  ...component,
  render: _self =>
    <UseRouterForLinks>
      <div className=Styles.outer>
        <div className=Styles.left> <MeasurableIndexSidebar channel /> </div>
        <div className=Styles.right>
          <div className=Styles.rightTop>
            <a href={"/c/" ++ channel} className=Styles.header>
              {"#" ++ channel |> ste}
            </a>
            {button(channel)}
          </div>
          <div className=Styles.rightBottom> children </div>
        </div>
      </div>
    </UseRouterForLinks>,
};