module Query = [%graphql
  {|
             mutation channelMembershipVerify (
                $input: ChannelMembershipVerifyInput!
             ) {
                 channelMembershipVerify(input: $input)
             }
     |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, ~agentId: string, ~channelId) => {
  let m =
    Query.make(
      ~input={
        "agentId": E.J.fromString(agentId),
        "channelId": E.J.fromString(channelId),
      },
      (),
    );
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"channelMemberships"|],
    (),
  )
  |> ignore;
};