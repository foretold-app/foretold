module Query = [%graphql
  {|
            mutation joinChannel($input: JoiningChannelInput!) {
                joinChannel(input: $input) {
                 agentId
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, channelId) => {
  let m =
    Query.make(~input={"channelId": Some(channelId), "role": `VIEWER}, ());
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getChannels", "user"|],
    (),
  )
  |> ignore;
};