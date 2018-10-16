open Antd.Grid;
open Utils;
open Rationale.Result.Infix;
open Antd;

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
  render: _ => {
    let input = userQuery =>
      <Row
        gutter=(Row.ResponsiveBreakpoints(makeGutterBreakpoints(~sm=5, ())))>
        <Col span=24>
          <Layout.Header className=Styles.header>
            <Antd_Menu className=Styles.menu mode=`Horizontal theme=`Light>
              <Antd_Menu.Item key="1">
                (link("/measurables", "Measurables"))
              </Antd_Menu.Item>
              <Antd_Menu.Item key="6">
                (link("/measurable-tables", "Measurable Tables"))
              </Antd_Menu.Item>
              <Antd_Menu.Item key="2">
                (link("/agents", "Agents"))
              </Antd_Menu.Item>
              (
                switch (userQuery) {
                | Some(_) =>
                  [|
                    <Antd_Menu.Item key="3">
                      (link("/measurables/new", "New Measurable"))
                    </Antd_Menu.Item>,
                    <Antd_Menu.Item key="4">
                      (link("/profile", "Profile"))
                    </Antd_Menu.Item>,
                    <Antd_Menu.Item key="5">
                      <a onClick=(_e => Auth0.logout())>
                        ("Log Out" |> ste)
                      </a>
                    </Antd_Menu.Item>,
                  |]
                  |> ReasonReact.array
                | _ =>
                  <Antd_Menu.Item key="6">
                    <a onClick=(_ => Auth0.logIn())> ("Login" |> ste) </a>
                  </Antd_Menu.Item>
                }
              )
            </Antd_Menu>
          </Layout.Header>
        </Col>
      </Row>;
    SharedQueries.withLoggedInUserQuery(input);
  },
};