module GraphQL = [%graphql
  {|
             mutation channelMembershipRoleUpdate($agentId: String!,$channelId:String, $role: ChannelMembershipRoles!) {
                 channelMembershipRoleUpdate(agentId: $agentId,channelId:$channelId, role: $role) {
                     agentId
                 }
             }
     |}
];

module Mutation = ReasonApollo.CreateMutation(GraphQL);

let mutate =
    (mutation: Mutation.apolloMutation, ~agentId: string, ~channelId, ~role) => {
  let m = GraphQL.make(~agentId, ~channelId, ~role, ());
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getChannelMemberships"|],
    (),
  )
  |> ignore;
};