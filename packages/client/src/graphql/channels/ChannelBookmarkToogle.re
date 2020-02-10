module Query = [%graphql
  {|
    mutation notebookDelete(
        $id: NotebookId!
    ) {
        notebookDelete(id: $id) {
         id
        }
    }
  |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, notebookId) => {
  let m = Query.make(~id=E.J.fromString(notebookId), ());
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"notebooks", "channel"|],
    (),
  )
  |> ignore;
};