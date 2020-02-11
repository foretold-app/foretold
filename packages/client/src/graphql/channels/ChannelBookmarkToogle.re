module Query = [%graphql
  {|
    mutation channelBookmarkToggle(
        $channelId: ChannelId!
    ) {
        channelBookmarkToggle(channelId: $channelId)
    }
  |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, channelId) => {
  let m = Query.make(~channelId=E.J.fromString(channelId), ());
  mutation(~variables=m##variables, ~refetchQueries=[||], ()) |> ignore;
};