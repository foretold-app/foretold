[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
    mutation notebookUpdate($id: String!, $input: NotebookUpdateInput!) {
        notebookUpdate(id: $id, input: $input) {
         id
        }
    }
  |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (
      mutation: Mutation.apolloMutation,
      id: string,
      name: string,
      body: string,
    ) => {
  let name = name |> E.J.fromString;
  let body = body |> E.J.fromString;

  let m = Query.make(~id, ~input={"name": name, "body": body}, ());

  mutation(~variables=m##variables, ~refetchQueries=[|"getNotebook"|], ())
  |> ignore;
};

let withMutation = innerComponentFn =>
  <Mutation onError={e => Js.log2("Graphql Error:", e)}>
    ...innerComponentFn
  </Mutation>;