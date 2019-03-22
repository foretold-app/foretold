module Query = [%graphql
  {|
            mutation agentsChannelsCreate($agentId: String!, $channelId: String!) {
                agentsChannelsCreate(agentId: $agentId, channelId: $channelId) {
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