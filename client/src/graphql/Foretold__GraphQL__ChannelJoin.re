module Query = [%graphql
  {|
            mutation channelMembershipCreate($agentId: String!, $channelId: String!) {
                channelMembershipCreate(agentId: $agentId, channelId: $channelId, role: VIEWER) {
                 agentId
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, agentId, channelId) => {
  let m = Query.make(~agentId, ~channelId, ());
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getChannels", "user"|],
    (),
  )
  |> ignore;
};