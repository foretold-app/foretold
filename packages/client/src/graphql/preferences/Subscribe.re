module Query = [%graphql
  {|
    mutation subscribe(
        $agentId: AgentId!
    ) {
        subscribe(
            agentId: $agentId
        )
    }
 |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);