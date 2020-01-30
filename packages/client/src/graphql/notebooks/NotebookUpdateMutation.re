module Query = [%graphql
  {|
    mutation notebookUpdate (
        $id: String!,
        $input: NotebookUpdateInput!
    ) {
        notebookUpdate (id: $id, input: $input) {
         id
        }
    }
  |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);