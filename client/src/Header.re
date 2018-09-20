open Antd.Grid;
open Antd.Layout;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("User");

let link = (url, name) =>
  <a onClick=(_e => ReasonReact.Router.push(url))>
    (name |> ReasonReact.string)
  </a>;

module Styles = {
  open Css;
  let header =
    style([
      width(`percent(100.0)),
      backgroundColor(white),
      selector(" h4", [marginTop(px(2))]),
    ]);

  let menu = style([lineHeight(px(64))]);
};

Css.(global("body", [fontFamily("Lato")]));

let make = _children => {
  ...component,
  render: _ =>
    <Row
      gutter=(Row.ResponsiveBreakpoints(makeGutterBreakpoints(~sm=5, ())))>
      <Col span=24>
        <Header className=Styles.header>
          <Antd_Menu className=Styles.menu mode=`Horizontal theme=`Light>
            <Antd_Menu.Item> (link("/agents", "Agents")) </Antd_Menu.Item>
            <Antd_Menu.Item>
              (link("/measurables", "Measurables"))
            </Antd_Menu.Item>
          </Antd_Menu>
        </Header>
      </Col>
    </Row>,
};