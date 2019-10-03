[@bs.config {jsx: 3}];

//   Internal(Agent({agentId: agent.id, subPage: AgentUpdates}))
[@react.component]
let make = (~agent: Types.agent) => <a />;

module Jsx2 = {
  let component = ReasonReact.statelessComponent("Link");

  let make =
      (
        ~linkType: LinkType.t,
        ~className: option(string)=?,
        children: array(ReasonReact.reactElement),
      ) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~linkType, ~className?, ~children, ()),
      children,
    );
};