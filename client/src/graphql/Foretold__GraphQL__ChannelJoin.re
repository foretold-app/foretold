module Query = [%graphql
  {|
            mutation channelMembershipCreate($input: ChannelMembershipRoleInput!) {
                channelMembershipCreate(input: $input) {
                 agentId
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, agentId, channelId) => {
  let m =
    Query.make(
      ~input={"agentId": agentId, "channelId": channelId, "role": `VIEWER},
      (),
    );
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getChannels", "user"|],
    (),
  )
  |> ignore;
};