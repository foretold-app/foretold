module Query = [%graphql
  {|
    mutation seriesUpdate (
        $id: String!,
        $input: SeriesUpdateInput!
    ) {
        seriesUpdate (id: $id, input: $input) {
         id
        }
    }
  |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);