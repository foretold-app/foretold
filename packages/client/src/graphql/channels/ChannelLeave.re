module Query = [%graphql
  {|
            mutation leaveChannel($input: JoiningChannelInput!) {
                leaveChannel(input: $input) {
                 agentId
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, channelId) => {
  let m = Query.make(~input={"channelId": Some(channelId)}, ());
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|
      "channels",
      "user",
      "channel",
      "channelMemberships",
    |],
    (),
  )
  |> ignore;
};