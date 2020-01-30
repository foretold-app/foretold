module Query = [%graphql
  {|
    mutation notebookCreate ($input: NotebookCreateInput!) {
        notebookCreate (input: $input) {
         id
        }
    }
  |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);