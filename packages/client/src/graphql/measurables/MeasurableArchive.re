module Query = [%graphql
  {|
             mutation measurableArchive($id: String!) {
                 measurableArchive(id: $id) {
                   id
                 }
             }
     |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, id: string) => {
  let m = Query.make(~id, ());
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