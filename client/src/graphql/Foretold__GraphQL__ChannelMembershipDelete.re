module Query = [%graphql
  {|
            mutation channelMembershipDelete($input: ChannelMembershipRoleInput!) {
                channelMembershipDelete(input: $input) {
                 agentId
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, agentId, channelId) => {
  let m =
    Query.make(
      ~input={
        "channelId": Some(channelId),
        "agentId": Some(agentId),
        "role": `VIEWER,
      },
      (),
    );
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getChannels", "user"|],
    (),
  )
  |> ignore;
};