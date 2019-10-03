[@bs.config {jsx: 3}];

let toOnClick = (agent: Types.agent) =>
  LinkType.onClick(
    Internal(Agent({agentId: agent.id, subPage: AgentUpdates})),
  );

let rec toAgent = (~agent: Types.agent) => {
  let onClick = toOnClick(agent);
  switch (agent.name, agent.agentType) {
  | (_, Some(User({name, picture}))) =>
    Some(FC__AgentLink.Agent.makeUser(~name, ~onClick, ~image=?picture, ()))
  | (_, Some(Bot({name, agent: Some(owner)}))) =>
    let owner = toAgent(~agent=owner);
    Some(
      FC__AgentLink.Agent.makeBot(
        ~name=name |> E.O.default("bot"),
        ~owner?,
        ~onClick,
        (),
      ),
    );
  | (_, Some(Bot({name}))) =>
    Some(
      FC__AgentLink.Agent.makeBot(
        ~name=name |> E.O.default("bot"),
        ~onClick,
        (),
      ),
    )
  | (Some(name), _) =>
    Some(FC__AgentLink.Agent.makeUser(~name, ~onClick, ()))
  | (_, _) => None
  };
};

[@react.component]
let make = (~agent: Types.agent) =>
  toAgent(~agent) |> E.O.React.fmapOrNull(agent => <FC__AgentLink agent />);

module Jsx2 = {
  let component = ReasonReact.statelessComponent("AgentLInk");

  let make = (~agent, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~agent, ()),
      children,
    );
};