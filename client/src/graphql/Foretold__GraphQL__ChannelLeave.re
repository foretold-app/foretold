module Query = [%graphql
  {|
            mutation agentsChannelsDelete($agentId: String!, $channelId: String!) {
                agentsChannelsDelete(agentId: $agentId, channelId: $channelId) {
                 agentId
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, agentId, channelId) => {
  let m = Query.make(~agentId, ~channelId, ());
  mutation(~variables=m##variables, ~refetchQueries=[|"getChannels"|], ())
  |> ignore;
};