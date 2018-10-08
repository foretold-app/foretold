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

let withUserQuery =
    (
      auth0Id: option(string),
      innerComponentFn: 'a => ReasonReact.reactElement,
    ) =>
  switch (auth0Id) {
  | Some(auth) =>
    let query = Queries.GetUser.make(~auth0Id=auth, ());
    Queries.GetUserQuery.make(~variables=query##variables, ({result}) =>
      result
      |> apolloResponseToResult
      <$> (e => innerComponentFn(Some(e)))
      |> Rationale.Result.result(idd, idd)
    )
    |> ReasonReact.element;
  | None => innerComponentFn(None)
  };

let make = _children => {
  ...component,
  render: _ => {
    let isLoggedIn = Auth0.isLoggedIn();
    Js.log(Auth0.userId());
    let input = userQuery =>
      <Row
        gutter=(Row.ResponsiveBreakpoints(makeGutterBreakpoints(~sm=5, ())))>
        <Col span=24>
          <Layout.Header className=Styles.header>
            <Antd_Menu className=Styles.menu mode=`Horizontal theme=`Light>
              <Antd_Menu.Item key="1">
                (link("/agents", "Agents"))
              </Antd_Menu.Item>
              <Antd_Menu.Item key="2">
                (link("/measurables", "Measurables"))
              </Antd_Menu.Item>
              <Antd_Menu.Item key="3">
                (link("/measurables/new", "New Measurable"))
              </Antd_Menu.Item>
              (
                switch (userQuery) {
                | Some(e) =>
                  [|
                    <Antd_Menu.Item key="4">
                      (
                        link(
                          "/profile",
                          Belt.Option.map(e##user, r => r##name)
                          |> Rationale.Option.default("Profile"),
                        )
                      )
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
    isLoggedIn ? withUserQuery(Auth0.userId(), input) : input(None);
  },
};