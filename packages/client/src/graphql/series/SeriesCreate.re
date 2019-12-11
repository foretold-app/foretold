module Query = [%graphql
  {|
            mutation seriesCreate($input:SeriesCreateInput!) {
                seriesCreate(input: $input) {
                  name
                }
            }
    |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);