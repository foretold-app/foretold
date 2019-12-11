module Query = [%graphql
  {|
             mutation measurableUpdate(
                $id: String!,
                $input: MeasurableUpdateInput!
             ) {
                 measurableUpdate(id: $id, input: $input) {
                   id
                   channelId
                 }
             }
     |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);