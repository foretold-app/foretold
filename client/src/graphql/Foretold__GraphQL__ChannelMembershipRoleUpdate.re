module GraphQL = [%graphql
  {|
             mutation channelMembershipRoleUpdate($input:ChannelMembershipRoleInput!) {
                 channelMembershipRoleUpdate(input: $input) {
                     agentId
                 }
             }
     |}
];

module Mutation = ReasonApollo.CreateMutation(GraphQL);

let mutate =
    (mutation: Mutation.apolloMutation, ~agentId: string, ~channelId, ~role) => {
  let m =
    GraphQL.make(
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