module Query = [%graphql
  {|
    mutation notebookBookmarkToggle(
        $notebookId: NotebookId!
    ) {
        notebookBookmarkToggle(notebookId: $notebookId)
    }
  |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, notebookId) => {
  let m = Query.make(~notebookId=E.J.fromString(notebookId), ());
  mutation(~variables=m##variables, ~refetchQueries=[|"notebooks"|], ())
  |> ignore;
};