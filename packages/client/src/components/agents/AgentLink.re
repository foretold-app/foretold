let agentLinkStyle = {
  Css.(
    style([
      color(FC__Settings.Text.LightBackground.light),
      hover([color(FC__Settings.Text.LightBackground.light)]),
    ])
  );
};

let toOnClick = (agent: Types.agent) =>
  LinkType.onClick(
    Internal(Agent({agentId: agent.id, subPage: AgentUpdates})),
  );

let rec toAgent = (~agent: Types.agent) => {
  let onClick = toOnClick(agent);

  switch (agent.name, agent.agentType) {
  | (_, Some(User({name, picture}))) =>
    Some(FC__AgentLink.Agent.makeUser(~name, ~onClick, ~image=?picture, ()))

  | (_, Some(Bot({name, owner: Some(agent), picture}))) =>
    let owner = toAgent(~agent);

    Some(
      FC__AgentLink.Agent.makeBot(
        ~name=name |> E.O.default("bot"),
        ~owner?,
        ~onClick,
        ~image=?picture,
        (),
      ),
    );

  | (_, Some(Bot({name, picture}))) =>
    Some(
      FC__AgentLink.Agent.makeBot(
        ~name=name |> E.O.default("bot"),
        ~onClick,
        ~image=?picture,
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
  toAgent(~agent)
  |> E.O.React.fmapOrNull(agent =>
       <FC__AgentLink agent className=agentLinkStyle />
     );

module Jsx2 = {
  let make = (~agent, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~agent, ()),
      children,
    );
};