module Query = [%graphql
  {|
            mutation channelMembershipDelete (
                $input: ChannelMembershipDeleteInput!
            ) {
                channelMembershipDelete (input: $input) {
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
        "channelId": E.J.fromString(channelId),
        "agentId": E.J.fromString(agentId),
        "role": `VIEWER,
      },
      (),
    );
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"channels", "user", "channel", "channelMemberships"|],
    (),
  )
  |> ignore;
};