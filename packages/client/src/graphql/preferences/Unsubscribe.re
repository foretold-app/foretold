module Query = [%graphql
  {|
    mutation unsubscribe(
        $agentId: AgentId!
    ) {
        unsubscribe(
            agentId: $agentId
        )
    }
 |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);