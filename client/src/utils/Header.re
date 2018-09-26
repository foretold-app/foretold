open Antd.Grid;
open Antd.Layout;
open Utils;
open Rationale.Result.Infix;

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
    let query = Queries.GetUser.make(~auth0Id="foobar", ());
    Queries.GetUserQuery.make(~variables=query##variables, ({result}) =>
      result
      |> apolloResponseToResult
      <$> (
        e =>
          <Row
            gutter=(
              Row.ResponsiveBreakpoints(makeGutterBreakpoints(~sm=5, ()))
            )>
            <Col span=24>
              <Header className=Styles.header>
                <Antd_Menu className=Styles.menu mode=`Horizontal theme=`Light>
                  <Antd_Menu.Item>
                    (link("/agents", "Agents"))
                  </Antd_Menu.Item>
                  <Antd_Menu.Item>
                    (link("/measurables", "Measurables"))
                  </Antd_Menu.Item>
                  <Antd_Menu.Item>
                    (link("/measurables/new", "New Measurable"))
                  </Antd_Menu.Item>
                  <Antd_Menu.Item>
                    (
                      Auth0.isLoggedIn() ?
                        <Antd_Menu.Item>
                          (
                            link(
                              "/measurables/new",
                              Belt.Option.map(e##user, r => r##name)
                              |> Rationale.Option.default("profile"),
                            )
                          )
                        </Antd_Menu.Item> :
                        <span onClick=(_ => Auth0.logIn())>
                          ("login" |> ste)
                        </span>
                    )
                  </Antd_Menu.Item>
                </Antd_Menu>
              </Header>
            </Col>
          </Row>
      )
      |> Rationale.Result.result(idd, idd)
    )
    |> ReasonReact.element;
  },
};