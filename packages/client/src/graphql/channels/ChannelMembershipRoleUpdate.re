module Query = [%graphql
  {|
             mutation channelMembershipRoleUpdate($input:ChannelMembershipRoleInput!) {
                 channelMembershipRoleUpdate(input: $input) {
                     agentId
                 }
             }
     |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (mutation: Mutation.apolloMutation, ~agentId: string, ~channelId, ~role) => {
  let m =
    Query.make(
      ~input={
        "agentId": Some(agentId),
        "channelId": Some(channelId),
        "role": role,
      },
      (),
    );
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getChannelMemberships"|],
    (),
  )
  |> ignore;
};