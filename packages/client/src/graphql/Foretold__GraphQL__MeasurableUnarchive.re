module GraphQL = [%graphql
  {|
             mutation measurableUnarchive($id: String!) {
                 measurableUnarchive(id: $id) {
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
    ~refetchQueries=[|
      "getAgent",
      "getMeasurables",
      "getMeasurablesStateStats",
    |],
    (),
  )
  |> ignore;
};