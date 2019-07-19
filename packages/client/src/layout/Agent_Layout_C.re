open SLayout;
open Utils;
open Style.Grid;

let component = ReasonReact.statelessComponent("AgentLayoutPage");

let make =
    (
      agentPage: Routing.AgentPage.t,
      loggedInUser: Primary.User.t,
      {head, body}: LayoutConfig.t,
    ) => {
  ...component,
  render: _ => {
    let agentId = agentPage.agentId;

    let top =
      GetAgent.component(
        ~id=agentId,
        ({agent}) => {
          let name =
            switch (agent) {
            | {bot: Some(r)} => r.name
            | {user: Some(r)} => r.name
            | _ => ""
            };

          let description =
            (
              switch (agent) {
              | {bot: Some(r)} => r.description
              | {user: Some(r)} => r.description
              | _ => Some("")
              }
            )
            |> E.O.default("");

          let score =
            switch (agent) {
            | {user: Some(r)} =>
              r.score == None
                ? "(none)" : r.score |> E.O.default(0.) |> string_of_float
            | _ => "(none)"
            };

          let secondLevel = AgentTabs.Component.tabs(agentPage, agent);

          <>
            <FC.GroupHeader>
              <div> {name |> ste} </div>
              <Div styles=[SLayout.Styles.descriptionText]>
                {"Percentage Question Brier Score: " |> ste}
                {score |> ste}
              </Div>
              <Div styles=[SLayout.Styles.descriptionText]>
                {description |> ste}
              </Div>
            </FC.GroupHeader>
            <FC.GroupHeader.SubHeader> secondLevel </FC.GroupHeader.SubHeader>
          </>;
        },
      );

    <Layout__Component__FillWithSidebar channelId=None loggedInUser>
      top
      <div className=Styles.container>
        <Div flexDirection=`row styles=[SLayout.Styles.width100]>
          <Div
            styles=[
              Css.(
                style([
                  marginTop(`em(1.0)),
                  paddingRight(`em(1.0)),
                  paddingLeft(`em(1.0)),
                ])
              ),
            ]
            flex=3>
            <FC.PageCard>
              <FC.PageCard.HeaderRow> head </FC.PageCard.HeaderRow>
              body
            </FC.PageCard>
          </Div>
        </Div>
      </div>
    </Layout__Component__FillWithSidebar>;
  },
};

let makeWithEl =
    (agentPage: Routing.AgentPage.t, loggedInUser, t: LayoutConfig.t) =>
  make(agentPage, loggedInUser, t) |> E.React.el;