[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
            mutation botCreate($input: BotInput!) {
                botCreate(input: $input) {
                 id
                }
            }
    |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);