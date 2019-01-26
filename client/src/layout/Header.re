open Antd.Grid;
open Antd;

let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("User");

let link = (url, name) =>
  <a onClick={_e => ReasonReact.Router.push(url)}>
    {name |> ReasonReact.string}
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
    let input: option(Queries.GetUser.Query.t) => ReasonReact.reactElement =
      userQuery =>
        <Row
          gutter={
            Row.ResponsiveBreakpoints(makeGutterBreakpoints(~sm=5, ()))
          }>
          <Col span=24>
            <Layout.Header className=Styles.header>
              <Antd_Menu className=Styles.menu mode=`Horizontal theme=`Light>
                <Antd_Menu.Item key="2">
                  {link("/measurables", "Measurables")}
                </Antd_Menu.Item>
                <Antd_Menu.Item key="1">
                  {link("/agents", "Agents")}
                </Antd_Menu.Item>
                <Antd_Menu.Item key="10">
                  <a
                    href="https://www.notion.so/Foretold-io-Wiki-c64b3b424c56485c8f40dbb77cbbb7de">
                    {"Wiki" |> ReasonReact.string}
                  </a>
                </Antd_Menu.Item>
                {
                  switch (userQuery) {
                  | Some(query) =>
                    open Rationale.Option.Infix;
                    let userAgentId = query##user >>= (e => e##agentId);
                    let idd =
                      switch (userAgentId) {
                      | Some(id) => id
                      | _ => ""
                      };
                    ();
                    [|
                      <Antd_Menu.Item key="3">
                        {link("/measurables/new", "New Measurable")}
                      </Antd_Menu.Item>,
                      <Antd_Menu.Item key="6">
                        {
                          link(
                            "/agents/" ++ idd ++ "/measurables",
                            "Edit Measurables",
                          )
                        }
                      </Antd_Menu.Item>,
                      <Antd_Menu.Item key="4">
                        {link("/profile", "Profile")}
                      </Antd_Menu.Item>,
                      <Antd_Menu.Item key="5">
                        <a onClick=(_e => Auth0.logout())>
                          {"Log Out" |> ste}
                        </a>
                      </Antd_Menu.Item>,
                    |]
                    |> ReasonReact.array;
                  | _ =>
                    <Antd_Menu.Item key="6">
                      <a onClick=(_ => Auth0.logIn())> {"Login" |> ste} </a>
                    </Antd_Menu.Item>
                  }
                }
              </Antd_Menu>
            </Layout.Header>
          </Col>
        </Row>;
    SharedQueries.withLoggedInUserQuery(input);
  },
};