module Query = [%graphql
  {|
            mutation createMeasurement($value: SequelizeJSON!, $description: String!, $competitorType:competitorType!, $measurableId:String!) {
                createMeasurement(value: $value, description: $description, competitorType: $competitorType, measurableId:$measurableId) {
                  createdAt
                }
            }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

type competitorType = [ | `COMPETITIVE | `OBJECTIVE];

let mutate =
    (
      mutation: Mutation.apolloMutation,
      measurableId: string,
      value: Value.t,
      competitorType: competitorType,
      description: string,
    ) => {
  let m =
    Query.make(
      ~measurableId,
      ~value=value |> Value.encode,
      ~description,
      ~competitorType,
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"getMeasurable"|], ())
  |> ignore;
};