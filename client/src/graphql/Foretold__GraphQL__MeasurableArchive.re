module GraphQL = [%graphql
  {|
             mutation unArchiveMeasurable($id: String!) {
                 unArchiveMeasurable(id: $id) {
                   id
                 }
             }
     |}
];

module Mutation = ReasonApollo.CreateMutation(GraphQL);

let mutate = (mutation: Mutation.apolloMutation, id: string) => {
  let m = GraphQL.make(~id, ());
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|"getAgent", "getMeasurables"|],
    (),
  )
  |> ignore;
};