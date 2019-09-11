open SLayout;
open Utils;
open Style.Grid;

let component = ReasonReact.statelessComponent("AgentLayoutPage");

let make =
    (
      agentPage: Routing.AgentPage.t,
      loggedInUser: Types.user,
      {head, body}: LayoutConfig.t,
    ) => {
  ...component,
  render: _ => {
    let agentId = agentPage.agentId;

    let top =
      AgentGet.component(
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
            <Fc.FC.GroupHeader>
              <div> {name |> ste} </div>
              <Div styles=[SLayout.Styles.descriptionText]>
                {"Percentage Question Brier Score: " |> ste}
                {score |> ste}
              </Div>
              <Div styles=[SLayout.Styles.descriptionText]>
                {description |> ste}
              </Div>
            </Fc.FC.GroupHeader>
            <Fc.FC.GroupHeader.SubHeader>
              secondLevel
            </Fc.FC.GroupHeader.SubHeader>
          </>;
        },
      );

    <FillWithSidebar loggedInUser>
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
            flex={`num(3.)}>
            <Fc.FC.PageCard>
              <Fc.FC.PageCard.HeaderRow> head </Fc.FC.PageCard.HeaderRow>
              body
            </Fc.FC.PageCard>
          </Div>
        </Div>
      </div>
    </FillWithSidebar>;
  },
};

let makeWithEl =
    (agentPage: Routing.AgentPage.t, loggedInUser, t: LayoutConfig.t) =>
  make(agentPage, loggedInUser, t) |> E.React.el;