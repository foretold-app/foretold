module Query = [%graphql
  {|
            mutation channelUpdate($id: String!, $input: ChannelInput!) {
                channelUpdate(id: $id, input: $input) {
                 id
                }
            }
    |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);