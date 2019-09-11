let component = ReasonReact.statelessComponent("AgentCommunities");
let make = (~agentId, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ => {
    let body =
      <Fc.FC.PageCard.Body>
        <ChannelTable.Jsx2 agentId={Some(agentId)} />
      </Fc.FC.PageCard.Body>;

    let head =
      <div>
        <Fc.FC.Base.Div float=`left>
          <Fc.FC.PageCard.HeaderRow.Title>
            {"Communities" |> ReasonReact.string}
          </Fc.FC.PageCard.HeaderRow.Title>
        </Fc.FC.Base.Div>
      </div>;

    SLayout.LayoutConfig.make(~head, ~body) |> layout;
  },
};